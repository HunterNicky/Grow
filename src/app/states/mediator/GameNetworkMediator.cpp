#include <cstdint>
#include <flatbuffers/verifier.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#include "chroma/app/states/GameState.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/app/states/network/PredictiveSyncSystem.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "entities_generated.h"
#include "events_generated.h"
#include "game_generated.h"
#include "messages_generated.h"

namespace chroma::app::states {
GameNetworkMediator::GameNetworkMediator(const std::shared_ptr<GameState> &game,
  const std::shared_ptr<NetworkState> &net)
  : game_state_(game), network_state_(net), interpolate_system_(std::make_unique<network::InterpolateSystem>()),
    predictive_sync_system_(std::make_unique<network::PredictiveSyncSystem>())
{}

GameNetworkMediator::GameNetworkMediator()
  : interpolate_system_(std::make_unique<network::InterpolateSystem>()),
    predictive_sync_system_(std::make_unique<network::PredictiveSyncSystem>())
{}

GameNetworkMediator::~GameNetworkMediator()
{
  game_state_.reset();
  network_state_.reset();
}

void GameNetworkMediator::OnSnapshotReceived(const std::vector<uint8_t> &data) const
{
  flatbuffers::Verifier verifier(data.data(), data.size());
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return; }
  const auto *envelope = Game::GetEnvelope(data.data());
  if (envelope == nullptr || envelope->type() != Game::MsgType::Snapshot) { return; }
  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  OnSnapshotReceived(snapshot);
}

void GameNetworkMediator::OnSnapshotReceived(const Game::Snapshot *snapshot) const
{
  if (snapshot == nullptr) { return; }
  const auto state = game_state_.lock();
  if (!state) { return; }

  const UUIDv4::UUID player_id = shared::packet::PacketHandler::SnapshotGetUUID(snapshot);

  
  auto manager = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager();
  auto new_snapshot = shared::packet::PacketHandler::SnapshotToGameObjects(manager, snapshot);
  
  if (state->GetPlayerId() != player_id) {
    state->SetPlayerId(player_id);
    interpolate_system_->SetPlayerId(player_id);
  }

  const uint32_t last_processed_input = shared::packet::PacketHandler::SnapshotGetLastProcessedInputSeq(snapshot);

  if (GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Exists(state->GetPlayerId())) {
    const auto player = std::static_pointer_cast<shared::core::player::Player>(
      GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Get(state->GetPlayerId()));

    if (player && predictive_sync_system_) {
      predictive_sync_system_->RemoveEventsAt(last_processed_input);
      predictive_sync_system_->ApplyEvents(player);
    }
  }

  interpolate_system_->OnPacketReceived(new_snapshot, shared::packet::PacketHandler::SnapshotGetTimeLapse(snapshot));

  ProcessPendingSoundEvents();
}

void GameNetworkMediator::OnEventReceived(const Game::Event *evt) const
{
  if (evt == nullptr) { return; }

  switch (evt->event_type()) {
  case Game::EventUnion::SoundEventMessage: {
    const auto *sound_msg = evt->event_as_SoundEventMessage();
    if ((sound_msg == nullptr) || (sound_msg->sound_event() == nullptr)) { return; }

    const auto *snd = sound_msg->sound_event();
    const std::string sound_id = (snd->sound_id() != nullptr) ? snd->sound_id()->str() : std::string();
    const float volume = snd->volume();
    const float pitch = snd->pitch();
    const std::string source_id = (snd->source_id() != nullptr) ? snd->source_id()->str() : std::string();

    shared::event::SoundEvent sound_event(sound_id, volume, pitch);
    sound_event.SetEmitterId(source_id);

    if (const auto state = game_state_.lock()) {
      bool delivered_to_emitter = false;
      if (!source_id.empty()) {
        UUIDv4::UUID const src_uuid(source_id);
        const auto manager = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager();
        if (manager && manager->Exists(src_uuid)) {
          const auto game_object = manager->Get(src_uuid);
          if (game_object) {
            game_object->HandleEvent(sound_event);
            delivered_to_emitter = true;
          }
        }
      }
      if (!delivered_to_emitter) { pending_sound_events_.push_back(sound_event); }
      state->OnEvent(sound_event);
    }
    break;
  }
  case Game::EventUnion::WaveEventMessage: {
    const auto *wave_msg = evt->event_as_WaveEventMessage();
    if (!wave_msg || !wave_msg->event()) { break; }

    const uint32_t wave_index = wave_msg->event()->wave_index();

    if (const auto state = game_state_.lock()) {
      (void)wave_index;
    }
    break;
  }
  default:
    break;
  }
}

void GameNetworkMediator::SendInput(const Game::InputEventMessage &input) { (void)input; }

void GameNetworkMediator::SetGameState(const std::shared_ptr<GameState> &game) { game_state_ = game; }

void GameNetworkMediator::SetNetworkState(const std::shared_ptr<NetworkState> &network) { network_state_ = network; }

[[nodiscard]] std::shared_ptr<GameState> GameNetworkMediator::GetGameState() const { return game_state_.lock(); }

[[nodiscard]] std::shared_ptr<NetworkState> GameNetworkMediator::GetNetworkState() const
{
  return network_state_.lock();
}

void GameNetworkMediator::AddInputEvent(const shared::event::Event &event) const
{
  if (event.GetType() == shared::event::Event::SoundEvent) { return; }
  if (predictive_sync_system_) { predictive_sync_system_->AddInputEventHistory(event); }
}

uint32_t GameNetworkMediator::GetSeqCounter() const
{
  if (predictive_sync_system_) { return predictive_sync_system_->GetSeqCounter(); }
  return 0;
}

void GameNetworkMediator::UpdateInterpolation(const uint64_t delta_time) const
{
  if (interpolate_system_) {
    auto manager = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager();
    if (manager) { interpolate_system_->Update(manager, delta_time); }
  }
}

void GameNetworkMediator::ProcessPendingSoundEvents() const
{
  if (pending_sound_events_.empty()) { return; }
  const auto manager = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager();
  if (!manager) { return; }

  std::vector<shared::event::SoundEvent> still_pending;
  still_pending.reserve(pending_sound_events_.size());

  for (auto &evt : pending_sound_events_) {
    const std::string emitter_id = evt.GetEmitterId();
    if (emitter_id.empty()) { continue; }
    UUIDv4::UUID const uuid_emitter(emitter_id);
    if (manager->Exists(uuid_emitter)) {
      const auto game_object = manager->Get(uuid_emitter);
      if (game_object) {
        game_object->HandleEvent(evt);
        continue;
      }
    }
    still_pending.push_back(evt);
  }
  pending_sound_events_.swap(still_pending);
}

}// namespace chroma::app::states
