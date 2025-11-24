#pragma once

#include <memory>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#include <flatbuffers/flatbuffer_builder.h>

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/packet/events/ProjectileMessage.h"
#include "entities_generated.h"

namespace chroma::server::logic {

class ServerGameLogic
{
public:
  ServerGameLogic();
  ~ServerGameLogic();

  ServerGameLogic(const ServerGameLogic &) = default;
  ServerGameLogic(ServerGameLogic &&) = delete;
  ServerGameLogic &operator=(const ServerGameLogic &) = default;
  ServerGameLogic &operator=(ServerGameLogic &&) = delete;

  static void CreateWorld();
  static void Update(float delta_time);

  static std::shared_ptr<shared::core::player::Player> CreatePlayer();
  static std::vector<flatbuffers::Offset<Game::EntityState>> GetEntitiesFlatBuffer(
    flatbuffers::FlatBufferBuilder &builder);

  static void OnReceivedInputMessage(const std::shared_ptr<shared::packet::InputMessage> &input_message,
    const UUIDv4::UUID &player_id);
  void OnReceivedProjectileMessage(const std::shared_ptr<shared::packet::ProjectileMessage> &projectile_message);
  static void HandleInput(shared::event::Event &event, const UUIDv4::UUID &player_id);
};

}// namespace chroma::server::logic
