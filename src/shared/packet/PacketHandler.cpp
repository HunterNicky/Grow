// #include "GameObject_generated.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include <cstdint>
#include <vector>
// #include <memory>


using namespace chroma::shared::core;

namespace chroma::server::net {

std::vector<uint8_t> PacketHandler::GameObjectToPacket(const chroma::shared::core::GameObject& object)
{
  (void)object;
  return {};
}

chroma::shared::core::GameObject* PacketHandler::PacketToGameObject(const uint8_t* data)
{
  (void)data;
  return nullptr;
}


} // namespace chroma::server::net
