#include "chroma/server/WorldSimulation.h"
#include "chroma/shared/core/player/Player.h"

#include <memory>

namespace chroma::server {
WorldSimulation::WorldSimulation() = default;

void WorldSimulation::CreateWorld() {
    
    auto player = std::make_shared<shared::core::player::Player>();

    game_objects_.emplace_back(player);
}

void WorldSimulation::Update(const float delta_time) {
    for (const auto& obj : game_objects_) {
        obj->OnUpdate(delta_time);
    }
}

} // namespace chroma::server