#include "chroma/server/core/GameServer.h"

#include <chrono>
#include <cstdint>
#include <enet.h>
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <memory>
#include <vector>

#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/packet/events/SoundEventMessage.h"
#include "entities_generated.h"
#include "events_generated.h"
#include "game_generated.h"
#include "messages_generated.h"

namespace chroma::server::core {

GameServer::GameServer()
{
  if (!InitServer(config_.port, config_.max_clients)) { return; }
  is_running_ = true;

  GCM::Instance().CreateContext(GameContextType::Server);

  if (shared::event::EventBus::GetDispatcher() == nullptr) {
    auto dispatcher = std::make_unique<shared::event::EventDispatcher>();
    shared::event::EventBus::SetDispatcher(dispatcher);
  }

  shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::SoundEvent>(
    [this](const shared::event::Event &ev) {
      if (!network_.IsReady()) { return; }

      const auto &sound_ev = dynamic_cast<const shared::event::SoundEvent &>(ev);

      const auto sound_msg = std::make_shared<shared::packet::SoundEventMessage>();
      sound_msg->SetSeq(0);
      sound_msg->SetDeltaTime(0.0F);
      sound_msg->SetSoundId(sound_ev.GetSoundName());
      sound_msg->SetVolume(sound_ev.GetVolume());
      sound_msg->SetPitch(sound_ev.GetPitch());
      sound_msg->SetSourceId(sound_ev.GetEmitterId());

      const auto buf = shared::packet::PacketHandler::SoundEventMessageToFlatBuffer(sound_msg);
      if (buf.empty()) { return; }

      for (const auto &[peer, session] : sessions_.GetAll()) {
        (void)session;
        network_.Send(peer, buf.data(), buf.size(), true);
      }
      network_.Flush();
    });
}

bool GameServer::InitServer(const int port, const int max_clients) { return network_.InitServer(port, max_clients); }

int GameServer::Start()
{
  if (!is_running_ || !network_.IsReady()) { return -1; }

  ENetEvent event;
  start_time_ = std::chrono::steady_clock::now();
  last_frame_time_ = start_time_;
  last_snapshot_time_ = start_time_;

  while (is_running_) {
    auto now = std::chrono::steady_clock::now();
    const auto frame_dt_sec = std::chrono::duration<float>(now - last_frame_time_).count();
    last_frame_time_ = now;

    while (is_running_ && network_.PollEvent(event, 1)) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT: {
        ConnectClient(event);
        break;
      }
      case ENET_EVENT_TYPE_DISCONNECT: {
        DisconnectClient(event);
        break;
      }
      case ENET_EVENT_TYPE_RECEIVE: {
        const auto *envelope = Game::GetEnvelope(event.packet->data);
        if (envelope == nullptr) { break; }

        if (envelope->type() == Game::MsgType::Event) {
          const Game::Event *evt = envelope->msg_as_Event();
          const auto event_union = evt->event_type();
          event_system_.ProcessGameEvent(event, 0, evt, event_union, game_logic_, sessions_);
        }
        break;
      }
      default:
        break;
      }
      enet_packet_destroy(event.packet);
    }

    const auto since_last_snapshot_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - last_snapshot_time_).count();
    if (since_last_snapshot_ms >= config_.ticks) {
      const auto server_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_).count();
      last_snapshot_time_ = now;
      BroadcastGameObjectState(static_cast<uint64_t>(server_time_ms));
    }

    game_logic_.Update(frame_dt_sec);
  }

  return 0;
}

int GameServer::Stop()
{
  network_.Stop();
  is_running_ = false;
  return 0;
}

void GameServer::ConnectClient(const ENetEvent &event)
{
  if (!is_running_ || event.peer == nullptr || event.type != ENET_EVENT_TYPE_CONNECT) { return; }

  const auto player = game_logic_.CreatePlayer();
  sessions_.AddSession(event.peer, player->GetId());

  flatbuffers::FlatBufferBuilder builder(1024);
  const std::vector<flatbuffers::Offset<Game::EntityState>> game_entities = game_logic_.GetEntitiesFlatBuffer(builder);

  const auto game_state =
    shared::packet::PacketHandler::GameObjectsToFlatBuffer(builder, game_entities, player->GetId(), 0, 0);

  network_.Send(event.peer, game_state.data(), game_state.size(), true);
  network_.Flush();
}

void GameServer::DisconnectClient(const ENetEvent &event)
{
  if (event.type != ENET_EVENT_TYPE_DISCONNECT) { return; }
  if (!is_running_ || !network_.IsReady()) { return; }

  if (network_.ConnectedPeers() == 0) { is_running_ = false; }

  sessions_.RemoveSession(event.peer);
}

void GameServer::BroadcastGameObjectState(const uint64_t server_time_ms) const
{
  if (!network_.IsReady()) { return; }

  for (const auto &[peer, session] : sessions_.GetAll()) {
    flatbuffers::FlatBufferBuilder builder(1024);
    const std::vector<flatbuffers::Offset<Game::EntityState>> game_entities =
      game_logic_.GetEntitiesFlatBuffer(builder);

    auto game_state = shared::packet::PacketHandler::GameObjectsToFlatBuffer(
      builder, game_entities, session.GetPlayerId(), server_time_ms, session.GetLastProcessedInput());

    network_.Send(peer, game_state.data(), game_state.size(), true);
  }

  network_.Flush();
}

}// namespace chroma::server::core
