#pragma once

#include <memory>

#include "GameObject_generated.h"

namespace chroma::app::states {

class GameState;
class NetworkState;

class GameNetworkMediator {
public:
    GameNetworkMediator() = default;
    GameNetworkMediator(const std::shared_ptr<GameState>& game, const std::shared_ptr<NetworkState>& net);

    static void OnSnapshotReceived(const std::unique_ptr<Game::Snapshot>& snapshot);

    static void SendInput(const Game::InputMessage& input);

    void SetGameState(const std::shared_ptr<GameState>& game);
    void SetNetworkState(const std::shared_ptr<NetworkState>& network);

    [[nodiscard]] std::shared_ptr<GameState> GetGameState() const;
    [[nodiscard]] std::shared_ptr<NetworkState> GetNetworkState() const;

private:
    std::weak_ptr<GameState> game_layer_;
    std::weak_ptr<NetworkState> network_layer_;
};

} // namespace chroma::app::states
