#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#include <flatbuffers/flatbuffer_builder.h>

#include "chroma/shared/packet/PacketHandler.h"

namespace chroma::server::network {

class ServerPacketHandler
{
public:
  static std::shared_ptr<shared::packet::InputMessage> EventToInputMessage(const Game::Event *evt)
  {
    return shared::packet::PacketHandler::EventToInputMessage(evt);
  }

  static std::vector<flatbuffers::Offset<Game::EntityState>> GameObjectsToFlatBufferEntities(
    flatbuffers::FlatBufferBuilder &builder,
    const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &objects)
  {
    return shared::packet::PacketHandler::GameObjectsToFlatBufferEntities(builder, objects);
  }

  static std::vector<uint8_t> BuildGameState(flatbuffers::FlatBufferBuilder &builder,
    const std::vector<flatbuffers::Offset<Game::EntityState>> &entities,
    const UUIDv4::UUID &player_id,
    const uint64_t time_lapse,
    const uint32_t last_processed_input)
  {
    return shared::packet::PacketHandler::GameObjectsToFlatBuffer(
      builder, entities, player_id, time_lapse, last_processed_input);
  }
};

}// namespace chroma::server::network
