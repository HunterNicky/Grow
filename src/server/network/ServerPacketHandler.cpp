#include "chroma/server/network/ServerPacketHandler.h"
#include "chroma/shared/packet/PacketHandler.h"

namespace chroma::server::network {

std::shared_ptr<shared::packet::InputMessage> ServerPacketHandler::EventToInputMessage(const Game::Event *evt)
{
  return shared::packet::PacketHandler::EventToInputMessage(evt);
}

std::shared_ptr<shared::packet::ProjectileMessage> ServerPacketHandler::EventToProjectileMessage(const Game::Event *evt)
{
  return shared::packet::PacketHandler::EventToProjectileMessage(evt);
}

std::vector<flatbuffers::Offset<Game::EntityState>> ServerPacketHandler::GameObjectsToFlatBufferEntities(
  flatbuffers::FlatBufferBuilder &builder,
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &objects)
{
  return shared::packet::PacketHandler::GameObjectsToFlatBufferEntities(builder, objects);
}

std::vector<uint8_t> ServerPacketHandler::BuildGameState(flatbuffers::FlatBufferBuilder &builder,
  const std::vector<flatbuffers::Offset<Game::EntityState>> &entities,
  const UUIDv4::UUID &player_id,
  const uint64_t time_lapse,
  const uint32_t last_processed_input)
{
  return shared::packet::PacketHandler::GameObjectsToFlatBuffer(
    builder, entities, player_id, time_lapse, last_processed_input);
}
}// namespace chroma::server::network
