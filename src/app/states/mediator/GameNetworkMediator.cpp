#include <cstdint>
#include <memory>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#include "chroma/app/states/GameState.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/app/states/network/PredictiveSyncSystem.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "game_generated.h"

namespace chroma::app::states {
GameNetworkMediator::GameNetworkMediator(const std::shared_ptr<GameState> &game,
  const std::shared_ptr<NetworkState> &net)
  : game_state_(game), network_state_(net),
    interpolate_system_(std::make_unique<network::InterpolateSystem>()),
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

void GameNetworkMediator::OnSnapshotReceived(const std::vector<uint8_t> &data)
{
  flatbuffers::Verifier verifier(data.data(), data.size());
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return; }
  const auto *envelope = Game::GetEnvelope(data.data());
  if (!envelope || envelope->type() != Game::MsgType::Snapshot) { return; }
  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  OnSnapshotReceived(snapshot);
}

void GameNetworkMediator::OnSnapshotReceived(const Game::Snapshot *snapshot)
{
  if (!snapshot) { return; }
  auto state = game_state_.lock();
  if (!state) { return; }

  const UUIDv4::UUID player_id = shared::packet::PacketHandler::SnapshotGetUUID(snapshot);
  state->SetPlayerId(player_id);
  interpolate_system_->SetPlayerId(player_id);

  auto game_objects = state->GetGameObjects();
  shared::packet::PacketHandler::SnapshotToGameObjects(snapshot, *game_objects);

  const uint32_t last_processed_input = shared::packet::PacketHandler::SnapshotGetLastProcessedInputSeq(snapshot);

  if (game_objects->contains(state->GetPlayerId())) {
    auto player = std::static_pointer_cast<shared::core::player::Player>((*game_objects)[state->GetPlayerId()]);

    if (player && predictive_sync_system_) {
      predictive_sync_system_->RemoveEventsAt(last_processed_input);
      predictive_sync_system_->ApplyEvents(player);
    }
  }
  interpolate_system_->Interpolate(*game_objects,
    shared::packet::PacketHandler::SnapshotGetTimeLapse(snapshot));
}

void GameNetworkMediator::OnEventReceived(const Game::Event *evt)
{
  if (!evt) { return; }
}

void GameNetworkMediator::SendInput(const Game::InputEventMessage &input) { (void)input; }

void GameNetworkMediator::SetGameState(const std::shared_ptr<GameState> &game) { game_state_ = game; }

void GameNetworkMediator::SetNetworkState(const std::shared_ptr<NetworkState> &network) { network_state_ = network; }

[[nodiscard]] std::shared_ptr<GameState> GameNetworkMediator::GetGameState() const { return game_state_.lock(); }

[[nodiscard]] std::shared_ptr<NetworkState> GameNetworkMediator::GetNetworkState() const
{
  return network_state_.lock();
}

void GameNetworkMediator::AddInputEvent(const shared::event::Event &event)
{
  if (predictive_sync_system_) { predictive_sync_system_->AddInputEventHistory(event); }
}

uint32_t GameNetworkMediator::GetSeqCounter() const
{
  if (predictive_sync_system_) { return predictive_sync_system_->GetSeqCounter(); }
  return 0;
}
void GameNetworkMediator::UpdateInterpolation(uint64_t delta_time)
{
  if (interpolate_system_) { interpolate_system_->Update(delta_time); }
}

void GameNetworkMediator::SetGameObjects(
  const std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>
    &game_objects)
{
  if (interpolate_system_) { interpolate_system_->SetGameObjects(game_objects); }
}
}// namespace chroma::app::states