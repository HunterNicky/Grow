#pragma once 

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/InputMessage.h"

#include <unordered_map>
#include <uuid_v4.h>
#include <memory>

namespace chroma::server {
class WorldSimulation {
public:
    WorldSimulation();
    ~WorldSimulation() = default;

    WorldSimulation(const WorldSimulation &) = default;
    WorldSimulation(WorldSimulation &&) = delete;
    WorldSimulation &operator=(const WorldSimulation &) = default;
    WorldSimulation &operator=(WorldSimulation &&) = delete;

    void CreateWorld();
    void Update(const float delta_time);

    std::shared_ptr<chroma::shared::core::player::Player> CreatePlayer();

    static void OnReceivedInputMessage(const std::shared_ptr<chroma::shared::packet::InputMessage>& input_message, const UUIDv4::UUID& player_id);
    static void HandleInput(shared::event::Event& event, const UUIDv4::UUID& player_id);

private:
    std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> game_objects_;
};
} // namespace chroma::server