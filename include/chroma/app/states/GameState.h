#pragma once

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"

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

  GameState(const GameState &) = delete;
  GameState &operator=(const GameState &) = delete;
  GameState(GameState &&) noexcept = delete;
  GameState &operator=(GameState &&) noexcept = delete;

  explicit GameState(std::shared_ptr<GameNetworkMediator> network_mediator);

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  void OnEvent(shared::event::Event &event) override;
  void HandleProjectileEvent(const shared::event::Event &event) const;

  void SetPlayerId(const UUIDv4::UUID &player_id);
  void SetEventDispatcher();

  [[nodiscard]] UUIDv4::UUID GetPlayerId() const;

private:
  std::shared_ptr<GameNetworkMediator> network_mediator_;
  UUIDv4::UUID player_id_;
  bool first_snapshot_received_{ true };
};
}// namespace chroma::app::states
