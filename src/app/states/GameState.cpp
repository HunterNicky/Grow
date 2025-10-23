#include "chroma/app/states/GameState.h"

 #include "chroma/shared/core/player/Player.h"
 #include "chroma/app/states/State.h"
 #include <memory>
#include <uuid_v4.h>

namespace chroma::app::states {

GameState::GameState() : State("GameState"), network_mediator_(nullptr) {
    
    auto player = std::make_shared<chroma::shared::core::player::Player>();
    SetPlayerId(player->GetId());
    player->InitComponents();
    game_objects_.emplace(player->GetId(), player);
}

GameState::GameState(std::shared_ptr<GameNetworkMediator> network_mediator)
    : State("GameState"), network_mediator_(std::move(network_mediator)) {
}

void GameState::OnRender() {

    if(!IsActive() || game_objects_.empty()) { return; }

    for (const auto &[uuid, obj] : game_objects_) {
        if (obj && obj->IsActive()) {
            obj->OnRender();
        }
    }
}

void GameState::OnUpdate(float delta_time) {

    if(!IsActive()) { return; }

    if(!network_mediator_) {
        for (const auto &[uuid, obj] : game_objects_) {
            if (obj && obj->IsActive()) {
                obj->OnUpdate(delta_time);
            }
        }
    }
}

void GameState::SetGameObjects(const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>& game_objects) {
    game_objects_ = game_objects;
}

[[nodiscard]] const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>& GameState::GetGameObjects() const {
    return game_objects_;
}

void GameState::OnEvent(shared::event::Event& event) {
    if(player_id_ == UUIDv4::UUID{}) {
        return;
    }

    auto it = game_objects_.find(player_id_);
    if (it != game_objects_.end()) {
        auto player = std::dynamic_pointer_cast<chroma::shared::core::player::Player>(it->second);
        player->HandleEvent(event);
    }
}

void GameState::SetPlayerId(const UUIDv4::UUID& player_id) {
    player_id_ = player_id;
}

UUIDv4::UUID GameState::GetPlayerId() const {
    return player_id_;
}

} // namespace chroma::app::states