#include "chroma/server/core/GameServer.h"

#include <chrono>
#include <flatbuffers/flatbuffer_builder.h>
#include <vector>

#include "chroma/shared/packet/PacketHandler.h"
#include "game_generated.h"

namespace chroma::server::core {

GameServer::GameServer()
{
  if (!InitServer(config_.port, config_.max_clients)) { return; }
  is_running_ = true;
}

bool GameServer::InitServer(const int port, const int max_clients)
{
  return network_.InitServer(port, max_clients);
}

int GameServer::Start()
{
  if (!is_running_ || !network_.IsReady()) { return -1; }

  ENetEvent event;
  auto last_tick = std::chrono::steady_clock::now();

  while (is_running_) {
    auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick).count();

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
          event_system_.ProcessGameEvent(event, elapsed, evt, event_union, game_logic_, sessions_);
        }
        break;
      }
      default:
        break;
      }
      enet_packet_destroy(event.packet);
    }

    tick_counter_++;

    if (tick_counter_ >= config_.ticks) {
      last_tick = now;
      tick_counter_ = 0;
      BroadcastGameObjectState(static_cast<uint64_t>(elapsed));
    }

    game_logic_.Update(static_cast<float>(elapsed) / 1000.0F);
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

  const auto game_state = shared::packet::PacketHandler::GameObjectsToFlatBuffer(builder, game_entities, player->GetId(), 0, 0);

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

void GameServer::BroadcastGameObjectState(const uint64_t delta_time)
{
  if (!network_.IsReady()) { return; }

  for (const auto &[peer, session] : sessions_.GetAll()) {
    flatbuffers::FlatBufferBuilder builder(1024);
    const std::vector<flatbuffers::Offset<Game::EntityState>> game_entities = game_logic_.GetEntitiesFlatBuffer(builder);

    auto game_state = shared::packet::PacketHandler::GameObjectsToFlatBuffer(
      builder, game_entities, session.GetPlayerId(), delta_time, session.GetLastProcessedInput());

    network_.Send(peer, game_state.data(), game_state.size(), true);
  }

  network_.Flush();
}

}// namespace chroma::server::core

