#pragma once

#include <cstdint>
#include <memory>
#include <sys/types.h>
#include <uuid_v4.h>
#include <vector>

#include "chroma/shared/events/SoundEvent.h"

#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/app/states/network/PredictiveSyncSystem.h"
#include "game_generated.h"

namespace chroma::app::states {

class GameState;
class NetworkState;

class GameNetworkMediator
{
public:
  GameNetworkMediator();
  GameNetworkMediator(const std::shared_ptr<GameState> &game, const std::shared_ptr<NetworkState> &net);
  ~GameNetworkMediator();

  GameNetworkMediator(const GameNetworkMediator &) = default;
  GameNetworkMediator &operator=(const GameNetworkMediator &) = default;
  GameNetworkMediator(GameNetworkMediator &&) noexcept = default;
  GameNetworkMediator &operator=(GameNetworkMediator &&) noexcept = default;

  void OnSnapshotReceived(const std::vector<uint8_t> &data) const;
  void OnSnapshotReceived(const Game::Snapshot *snapshot) const;
  void OnEventReceived(const Game::Event *evt) const;

  static void SendInput(const Game::InputEventMessage &input);

  void SetGameState(const std::shared_ptr<GameState> &game);
  void SetNetworkState(const std::shared_ptr<NetworkState> &network);
  [[nodiscard]] std::shared_ptr<GameState> GetGameState() const;
  [[nodiscard]] std::shared_ptr<NetworkState> GetNetworkState() const;

  void AddInputEvent(const shared::event::Event &event) const;
  [[nodiscard]] uint32_t GetSeqCounter() const;

  void UpdateInterpolation(uint64_t delta_time) const;

  void ProcessPendingSoundEvents() const;

private:
  void HandleSoundEvent(const Game::Event *evt) const;
  void HandleWaveEvent(const Game::Event *evt) const;

  std::weak_ptr<GameState> game_state_;
  std::weak_ptr<NetworkState> network_state_;
  std::unique_ptr<network::InterpolateSystem> interpolate_system_;
  std::unique_ptr<network::PredictiveSyncSystem> predictive_sync_system_;
  mutable std::vector<shared::event::SoundEvent> pending_sound_events_;
};

}// namespace chroma::app::states
