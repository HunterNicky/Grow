#include <cstdint>
#include <memory>
#include <sys/types.h>
#include <uuid_v4.h>
#include <vector>

#include "GameObject_generated.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/shared/packet/PacketHandler.h"

namespace chroma::app::states {
GameNetworkMediator::GameNetworkMediator(const std::shared_ptr<GameState> &game,
  const std::shared_ptr<NetworkState> &net)
  : game_state_(game), network_state_(net), interpolate_system_(std::make_unique<chroma::app::states::network::InterpolateSystem>()),
    predictive_sync_system_(std::make_unique<chroma::app::states::network::PredictiveSyncSystem>())
{}

GameNetworkMediator::GameNetworkMediator() : interpolate_system_(std::make_unique<chroma::app::states::network::InterpolateSystem>()),
    predictive_sync_system_(std::make_unique<chroma::app::states::network::PredictiveSyncSystem>())
{}

GameNetworkMediator::~GameNetworkMediator()
{
  game_state_.reset();
  network_state_.reset();
}

void GameNetworkMediator::OnSnapshotReceived(const std::vector<uint8_t> &data)
{
  auto state = game_state_.lock();
  if (!state) { return; }

  const UUIDv4::UUID player_id = shared::packet::PacketHandler::FlatBufferSnapshotGetUUID(data.data(), data.size());
  state->SetPlayerId(player_id);
  interpolate_system_->SetPlayerId(player_id);

  auto game_objects = state->GetGameObjects();
  chroma::shared::packet::PacketHandler::FlatBufferToGameObject(data.data(), data.size(), *game_objects);

  uint32_t last_processed_input = shared::packet::PacketHandler::FlatBufferSnapshotGetLastProcessedInputSeq(data.data(), data.size());

  if (game_objects->contains(state->GetPlayerId())) {
      auto player = std::dynamic_pointer_cast<chroma::shared::core::player::Player>((*game_objects)[state->GetPlayerId()]);

      if (player && predictive_sync_system_) {

        predictive_sync_system_->RemoveEventsAt(last_processed_input);
        predictive_sync_system_->ApplyEvents(player);
      }
  }
  if (interpolate_system_) {
      interpolate_system_->Interpolate(*game_objects, shared::packet::PacketHandler::FlatBufferSnapshotGetTimeLapse(data.data(), data.size()));
  }
}

void GameNetworkMediator::SendInput(const Game::InputMessage &input) { (void)input; }

void GameNetworkMediator::SetGameState(const std::shared_ptr<GameState> &game) { game_state_ = game; }

void GameNetworkMediator::SetNetworkState(const std::shared_ptr<NetworkState> &network) { network_state_ = network; }

[[nodiscard]] std::shared_ptr<GameState> GameNetworkMediator::GetGameState() const { return game_state_.lock(); }

[[nodiscard]] std::shared_ptr<NetworkState> GameNetworkMediator::GetNetworkState() const
{
  return network_state_.lock();
}

void GameNetworkMediator::AddInputEvent(const shared::event::Event &event)
{
  if (predictive_sync_system_) {
    predictive_sync_system_->AddInputEventHistory(event);
  }
}

uint32_t GameNetworkMediator::GetSeqCounter() const
{
  if (predictive_sync_system_) {
    return predictive_sync_system_->GetSeqCounter();
  }
  return 0;
}
void GameNetworkMediator::UpdateInterpolation(uint64_t delta_time)
{
  if (interpolate_system_) {
    interpolate_system_->Update(delta_time);
  }
}

void GameNetworkMediator::SetGameObjects(const std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>>& game_objects)
{
  if (interpolate_system_) {
      interpolate_system_->SetGameObjects(game_objects);
  }
}
} // namespace chroma::app::states