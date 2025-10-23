#include "chroma/server/WorldSimulation.h"
#include "chroma/shared/core/player/Player.h"

#include <memory>
#include <uuid_v4.h>

namespace chroma::server {
WorldSimulation::WorldSimulation() = default;

void WorldSimulation::CreateWorld() {
    //Criar mundo do jogo
}

void WorldSimulation::Update(const float delta_time) {
    for (const auto& [id, obj] : game_objects_) {
        obj->OnUpdate(delta_time);
    }
}

void WorldSimulation::OnReceivedInputMessage(const std::shared_ptr<chroma::shared::packet::InputMessage>& input_message, const UUIDv4::UUID& player_id) {
    (void)input_message;
    (void)player_id;
}

std::shared_ptr<chroma::shared::core::player::Player> WorldSimulation::CreatePlayer() {
    auto player = std::make_shared<chroma::shared::core::player::Player>();
    player->InitComponents();
    game_objects_.emplace(player->GetId(), player);
    return player;
}

void WorldSimulation::HandleInput(shared::event::Event& event, const UUIDv4::UUID& player_id) {
    (void)event;
    (void)player_id;
}
} // namespace chroma::server