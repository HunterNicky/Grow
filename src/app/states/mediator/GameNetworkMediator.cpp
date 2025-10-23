#include "chroma/app/states/mediator/GameNetworkMediator.h"

namespace chroma::app::states {
    GameNetworkMediator::GameNetworkMediator(const std::shared_ptr<GameState>& game, const std::shared_ptr<NetworkState>& net)
    : game_layer_(game), network_layer_(net)
    {
    }

    void GameNetworkMediator::OnSnapshotReceived(const std::unique_ptr<Game::Snapshot>& snapshot) {
        (void)snapshot;
    }

    void GameNetworkMediator::SendInput(const Game::InputMessage& input) {
        (void)input;
    }

    void GameNetworkMediator::SetGameState(const std::shared_ptr<GameState>& game) {
        game_layer_ = game;
    }

    void GameNetworkMediator::SetNetworkState(const std::shared_ptr<NetworkState>& network) {
        network_layer_ = network;
    }

    [[nodiscard]] std::shared_ptr<GameState> GameNetworkMediator::GetGameState() const {
        return game_layer_.lock();
    }

    [[nodiscard]] std::shared_ptr<NetworkState> GameNetworkMediator::GetNetworkState() const {
        return network_layer_.lock();
    }

}