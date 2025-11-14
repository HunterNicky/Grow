#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <unordered_map>
#include <uuid_v4.h>
#include <cstdint>
#include <memory>
#include <vector>

#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma//shared/core/GameObject.h"

#include "entities_generated.h"
#include "events_generated.h"

namespace chroma::server::network {

class ServerPacketHandler
{
public:
  static std::shared_ptr<shared::packet::InputMessage> EventToInputMessage(const Game::Event *evt);

  static std::vector<flatbuffers::Offset<Game::EntityState>> GameObjectsToFlatBufferEntities(
    flatbuffers::FlatBufferBuilder &builder,
    const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &objects);

  static std::vector<uint8_t> BuildGameState(flatbuffers::FlatBufferBuilder &builder,
    const std::vector<flatbuffers::Offset<Game::EntityState>> &entities,
    const UUIDv4::UUID &player_id,
    const uint64_t time_lapse,
    const uint32_t last_processed_input);
};

}// namespace chroma::server::network
