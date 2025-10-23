#include "chroma/server/WorldSimulation.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/PacketHandler.h"

#include <memory>
#include <uuid_v4.h>

namespace chroma::server {
WorldSimulation::WorldSimulation() = default;

void WorldSimulation::CreateWorld() {
    //Criar mundo do jogo
}

WorldSimulation::~WorldSimulation()
{
    game_objects_.clear();
}

void WorldSimulation::Update(const float delta_time) {
    for (const auto& [id, obj] : game_objects_) {
        obj->OnUpdate(delta_time);
    }
}

void WorldSimulation::OnReceivedInputMessage(const std::shared_ptr<chroma::shared::packet::InputMessage>& input_message, const UUIDv4::UUID& player_id) {
    std::shared_ptr<shared::event::Event> event = input_message->GetEvent();
    HandleInput(*event, player_id);
}

std::shared_ptr<chroma::shared::core::player::Player> WorldSimulation::CreatePlayer() {
    auto player = std::make_shared<chroma::shared::core::player::Player>();
    player->InitComponents();
    game_objects_.emplace(player->GetId(), player);
    return player;
}

void WorldSimulation::HandleInput(shared::event::Event& event, const UUIDv4::UUID& player_id) {
    auto it = game_objects_.find(player_id);
    if (it != game_objects_.end()) {
        auto player = std::dynamic_pointer_cast<chroma::shared::core::player::Player>(it->second);
        if (player) {
            player->HandleEvent(event);
        }
    }
}

std::vector<uint8_t> WorldSimulation::GetGameStateSnapshot(const UUIDv4::UUID& player_id) const {
    return shared::packet::PacketHandler::GameObjectsToFlatBuffer(game_objects_, player_id, 0, 0);
}
} // namespace chroma::server