#pragma once

#include <cstdint>
#include <memory>
#include <uuid_v4.h>
#include <vector>

#include "GameObject_generated.h"

namespace chroma::app::states {

class GameState;
class NetworkState;

class GameNetworkMediator {
public:
    GameNetworkMediator() = default;
    GameNetworkMediator(const std::shared_ptr<GameState>& game, const std::shared_ptr<NetworkState>& net);
    ~GameNetworkMediator();

    GameNetworkMediator(const GameNetworkMediator&) = default;
    GameNetworkMediator& operator=(const GameNetworkMediator&) = default;
    GameNetworkMediator(GameNetworkMediator&&) noexcept = default;
    GameNetworkMediator& operator=(GameNetworkMediator&&) noexcept = default;

    void OnSnapshotReceived(const std::vector<uint8_t>& data);

    static void SendInput(const Game::InputMessage& input);

    void SetGameState(const std::shared_ptr<GameState>& game);
    void SetNetworkState(const std::shared_ptr<NetworkState>& network);
    void RegisterPlayerId(const UUIDv4::UUID& player_id);
    [[nodiscard]] std::shared_ptr<GameState> GetGameState() const;
    [[nodiscard]] std::shared_ptr<NetworkState> GetNetworkState() const;

private:
    std::weak_ptr<GameState> game_state_;
    std::weak_ptr<NetworkState> network_state_;
};

} // namespace chroma::app::states
