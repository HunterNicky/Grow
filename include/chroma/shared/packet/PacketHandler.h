#pragma once

#pragma once

#include "chroma/shared/core/GameObject.h"
#include <memory>
#include <vector>

namespace chroma::server::net {

class PacketHandler {
public:
  static std::vector<uint8_t> GameObjectToPacket(const chroma::shared::core::GameObject& object);

  static chroma::shared::core::GameObject* PacketToGameObject(const uint8_t* data);
};

} // namespace chroma::server::net
