#pragma once 

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/packet/ImputMessage.h"

#include <vector>
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
    static void OnReceivedImputMessage(const std::shared_ptr<chroma::shared::packet::InputMessage>& input_message);
    void HandleInput();
    void Update(const float delta_time);

private:
    std::vector<std::shared_ptr<shared::core::GameObject>> game_objects_;
};
} // namespace chroma::server