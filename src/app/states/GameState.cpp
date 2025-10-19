#include "chroma/app/states/GameState.h"

 #include "chroma/shared/core/player/Player.h"
 #include "chroma/app/states/State.h"
 #include <memory>

namespace chroma::app::layer::states {

GameState::GameState() : State("GameState"), network_mediator_(nullptr) {
    
    auto player = std::make_shared<chroma::shared::core::player::Player>();
    player->InitComponents();
    game_objects_.emplace_back(player);
}

GameState::GameState(std::shared_ptr<GameNetworkMediator> network_mediator)
    : State("GameState"), network_mediator_(std::move(network_mediator)) {
}


void GameState::OnRender() {

    if(!IsActive()) { return; }
    
    for (const auto &obj : game_objects_) {
        if (obj && obj->IsActive()) {
            obj->OnRender();
        }
    }
}

void GameState::OnUpdate(float delta_time) {

    if(!IsActive()) { return; }

    if(!network_mediator_) {
        for (const auto &obj : game_objects_) {
            if (obj && obj->IsActive()) {
                obj->OnUpdate(delta_time);
            }
        }
    }
}

} // namespace chroma::app::layer::states