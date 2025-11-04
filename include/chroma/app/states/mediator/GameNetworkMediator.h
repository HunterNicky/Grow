#pragma once

#include <cstdint>
#include <memory>
#include <sys/types.h>
#include <uuid_v4.h>
#include <vector>

#include "GameObject_generated.h"
#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/app/states/network/PredictiveSyncSystem.h"

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

  void OnSnapshotReceived(const std::vector<uint8_t> &data);

  static void SendInput(const Game::InputMessage &input);

  void SetGameState(const std::shared_ptr<GameState> &game);
  void SetNetworkState(const std::shared_ptr<NetworkState> &network);
  [[nodiscard]] std::shared_ptr<GameState> GetGameState() const;
  [[nodiscard]] std::shared_ptr<NetworkState> GetNetworkState() const;

  void AddInputEvent(const shared::event::Event &event);
  [[nodiscard]] uint32_t GetSeqCounter() const;

  void UpdateInterpolation(uint64_t delta_time);
  void SetGameObjects(
    const std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>>
      &game_objects);

private:
  std::weak_ptr<GameState> game_state_;
  std::weak_ptr<NetworkState> network_state_;
  std::unique_ptr<chroma::app::states::network::InterpolateSystem> interpolate_system_;
  std::unique_ptr<chroma::app::states::network::PredictiveSyncSystem> predictive_sync_system_;
};

}// namespace chroma::app::states
