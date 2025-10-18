#pragma once

#include <GameObject_generated.h>
#include <memory>
#include <vector>
#include "chroma/shared/core/GameObject.h"

namespace chroma::server::packet {

class PacketHandler {
public:
    static std::vector<uint8_t> GameObjectToFlatBuffer(const chroma::shared::core::GameObject& object);

    static std::unique_ptr<chroma::shared::core::GameObject> FlatBufferToGameObject(const uint8_t* data, size_t size);
};

} // namespace chroma::server::packet
