#pragma once

#include <memory>

#include "GameObject_generated.h"

namespace chroma::app::layer {

class GameLayer;
class NetworkLayer;

class GameNetworkMediator {
public:
    GameNetworkMediator(std::shared_ptr<GameLayer> game, std::shared_ptr<NetworkLayer> net);

    void OnSnapshotReceived(const Game::Snapshot* snapshot);

    void SendInput(const Game::InputMessage& input);

private:
    std::weak_ptr<GameLayer> game_layer_;
    std::weak_ptr<NetworkLayer> network_layer_;
    
};

} // namespace chroma
