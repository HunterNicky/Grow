#pragma once

#include <GameObject_generated.h>
#include <cstdint>
#include <memory>
#include <vector>

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/packet/ImputMessage.h"

namespace chroma::server::packet {

class PacketHandler {
public:
    static std::vector<uint8_t> GameObjectsToFlatBuffer(const std::vector<std::shared_ptr<chroma::shared::core::GameObject>>& objects,  uint64_t time_lapse = 0, uint32_t last_processed_input = 0);
    static void FlatBufferToGameObject(const uint8_t* data, std::vector<std::shared_ptr<chroma::shared::core::GameObject>>& game_objects);
    static std::shared_ptr<shared::packet::InputMessage> FlatBufferToInputMessage(const uint8_t* data);
private:
    static void UpdateGameObjectWithEntityState(const Game::EntityState* entity_state, std::shared_ptr<chroma::shared::core::GameObject>& game_object);
    static void ComponentToSpeed(const Game::Component* component, std::shared_ptr<chroma::shared::core::GameObject>& game_object);
    static void ComponentToTransform(const Game::Component* component, std::shared_ptr<chroma::shared::core::GameObject>& game_object);
};

} // namespace chroma::server::packet
