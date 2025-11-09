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
#include "game_generated.h"

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
  void Update(float delta_time) const;

  std::shared_ptr<shared::core::player::Player> CreatePlayer();
  std::vector<flatbuffers::Offset<Game::EntityState>> GetEntitiesFlatBuffer(
    flatbuffers::FlatBufferBuilder &builder) const;

  void OnReceivedInputMessage(const std::shared_ptr<shared::packet::InputMessage> &input_message,
    const UUIDv4::UUID &player_id);
  void HandleInput(const shared::event::Event &event, const UUIDv4::UUID &player_id);

  [[nodiscard]] const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &
    GetGameObjects() const
  {
    return game_objects_;
  }

private:
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> game_objects_;
};

}// namespace chroma::server::logic
