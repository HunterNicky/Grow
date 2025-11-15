#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/packet/events/SoundEventMessage.h"
#include "game_generated.h"

namespace chroma::shared::packet {

class PacketHandler
{
public:
  //
  // ============================================================
  //  SERIALIZAÇÃO
  // ============================================================
  //
  static std::vector<uint8_t> GameObjectsToFlatBuffer(flatbuffers::FlatBufferBuilder &builder,
    const std::vector<flatbuffers::Offset<Game::EntityState>> &entities,
    const UUIDv4::UUID &player_id,
    uint64_t time_lapse = 0,
    uint32_t last_processed_input = 0);
  static std::vector<flatbuffers::Offset<Game::EntityState>> GameObjectsToFlatBufferEntities(
    flatbuffers::FlatBufferBuilder &builder,
    const std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>> &objects);
  static std::vector<uint8_t> InputMessageToFlatBuffer(const std::shared_ptr<InputMessage> &input_message);
  static std::vector<uint8_t> SoundEventMessageToFlatBuffer(const std::shared_ptr<SoundEventMessage> &sound_message);

  //
  // ============================================================
  //  SNAPSHOT
  // ============================================================
  //
  static UUIDv4::UUID SnapshotGetUUID(const Game::Snapshot *snapshot);
  static uint32_t SnapshotGetLastProcessedInputSeq(const Game::Snapshot *snapshot);
  static uint64_t SnapshotGetTimeLapse(const Game::Snapshot *snapshot);
  static void SnapshotToGameObjects(const Game::Snapshot *snapshot,
   std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>> &game_objects);


  //
  // ============================================================
  //  EVENTOS
  // ============================================================
  //
  static std::shared_ptr<InputMessage> EventToInputMessage(const Game::Event *evt);

private:
  static void UpdateGameObjectWithEntityState(const Game::EntityState *entity_state,
    std::shared_ptr<core::GameObject> &game_object);
};

}// namespace chroma::shared::packet
