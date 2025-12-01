#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/packet/events/ProjectileMessage.h"
#include "chroma/shared/packet/events/SoundEventMessage.h"
#include "entities_generated.h"
#include "events_generated.h"


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
    flatbuffers::FlatBufferBuilder &builder);
  static std::vector<uint8_t> InputMessageToFlatBuffer(const std::shared_ptr<InputMessage> &input_message);
  static std::vector<uint8_t> SoundEventMessageToFlatBuffer(const std::shared_ptr<SoundEventMessage> &sound_message);
  static std::vector<uint8_t> ProjectileMessageToFlatBuffer(
    const std::shared_ptr<ProjectileMessage> &projectile_message);
  static std::vector<uint8_t> WaveEventMessageToFlatBuffer(uint32_t seq, float dt, uint32_t wave_index);
  //
  // ============================================================
  //  SNAPSHOT
  // ============================================================
  //
  static UUIDv4::UUID SnapshotGetUUID(const Game::Snapshot *snapshot);
  static uint32_t SnapshotGetLastProcessedInputSeq(const Game::Snapshot *snapshot);
  static uint64_t SnapshotGetTimeLapse(const Game::Snapshot *snapshot);
  static std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>>
    SnapshotToGameObjects(const std::shared_ptr<core::GameObjectManager> &manager, const Game::Snapshot *snapshot);


  //
  // ============================================================
  //  EVENTOS
  // ============================================================
  //
  static std::shared_ptr<InputMessage> EventToInputMessage(const Game::Event *evt);
  static std::shared_ptr<ProjectileMessage> EventToProjectileMessage(const Game::Event *evt);

private:
  static void UpdateGameObjectWithEntityState(const Game::EntityState *entity_state,
    const std::shared_ptr<core::GameObject> &game_object);

  static std::shared_ptr<core::GameObject> GetOrCreateObjectFromState(
    const std::shared_ptr<core::GameObjectManager> &manager,
    std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>> &current_clones,
    const Game::EntityState *entity_state,
    bool is_local_player);
};

}// namespace chroma::shared::packet
