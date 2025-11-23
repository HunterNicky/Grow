#include "chroma/server/network/ServerPacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/packet/events/ProjectileMessage.h"
#include "entities_generated.h"
#include "events_generated.h"

#include <cstdint>
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <memory>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

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
