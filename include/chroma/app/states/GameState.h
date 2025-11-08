#pragma once

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/EventDispatcher.h"

#include <memory>
#include <unordered_map>
#include <uuid_v4.h>

enum SelectLevel : uint8_t {
  MeadowLevel = 0,
};

namespace chroma::app::states {
class GameState : public State
{
public:
  GameState();
  ~GameState() override;

  GameState(const GameState &) = default;
  GameState &operator=(const GameState &) = default;
  GameState(GameState &&) noexcept = delete;
  GameState &operator=(GameState &&) noexcept = delete;

  explicit GameState(std::shared_ptr<GameNetworkMediator> network_mediator);

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  void OnEvent(shared::event::Event &event) override;

  void SetPlayerId(const UUIDv4::UUID &player_id);
  void SetEventDispatcher();
  void SetGameObjects(
    const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &game_objects);

  [[nodiscard]] UUIDv4::UUID GetPlayerId() const;
  [[nodiscard]] std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>> &
    GetGameObjects();
  [[nodiscard]] std::shared_ptr<shared::core::player::Player> GetPlayer() const;

private:
  std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>> game_objects_;
  std::shared_ptr<GameNetworkMediator> network_mediator_;
  UUIDv4::UUID player_id_;
};
}// namespace chroma::app::states
