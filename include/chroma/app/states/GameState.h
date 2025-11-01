#pragma once

#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/app/states/State.h"

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
  GameState(GameState &&) noexcept = default;
  GameState &operator=(GameState &&) noexcept = default;

  explicit GameState(std::shared_ptr<chroma::app::states::GameNetworkMediator> network_mediator);
  explicit GameState(std::shared_ptr<chroma::shared::event::EventDispatcher> event_dispatcher);

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  void OnEvent(shared::event::Event &event) override;

  void SetPlayerId(const UUIDv4::UUID &player_id);
  void SetEventDispatcher(const std::shared_ptr<chroma::shared::event::EventDispatcher> &event_dispatcher);
  void SetGameObjects(
    const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> &game_objects);

  UUIDv4::UUID GetPlayerId() const;
  [[nodiscard]] const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> &
    GetGameObjects() const;
  std::shared_ptr<chroma::shared::core::player::Player> GetPlayer();

private:
std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> game_objects_;
std::shared_ptr<chroma::app::states::GameNetworkMediator> network_mediator_;
std::shared_ptr<chroma::shared::event::EventDispatcher> event_dispatcher_;
UUIDv4::UUID player_id_;
};
}// namespace chroma::app::states
