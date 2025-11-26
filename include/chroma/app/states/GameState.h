#pragma once

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/shared/core/components/CharacterType.h"

#include <common_generated.h>
#include <memory>
#include <uuid_v4.h>

enum SelectLevel : uint8_t {
  MeadowLevel = 0,
};

namespace chroma::app::states {
class GameState : public State
{
public:
  GameState(shared::core::component::CharacterType character_type = shared::core::component::CharacterType::NONE);
  ~GameState() override;

  GameState(const GameState &) = delete;
  GameState &operator=(const GameState &) = delete;
  GameState(GameState &&) noexcept = delete;
  GameState &operator=(GameState &&) noexcept = delete;

  explicit GameState(std::shared_ptr<GameNetworkMediator> network_mediator);

  static void CreatePlayerWithPlayerData(const app::database::PlayerData &player_data);

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  void OnEvent(shared::event::Event &event) override;
  static void HandleProjectileEvent(const shared::event::Event &event);

  void SetPlayerId(const UUIDv4::UUID &player_id);
  void SetEventDispatcher();
  void SetSoundEventDispatcher();

  [[nodiscard]] UUIDv4::UUID GetPlayerId() const;

  [[nodiscard]] bool IsPaused() const;
  void SetPaused(bool paused);

private:
  std::shared_ptr<GameNetworkMediator> network_mediator_;
  UUIDv4::UUID player_id_;
  bool first_snapshot_received_{ true };

  shared::event::Subscription key_sub_;
  shared::event::Subscription projectile_sub_;
  shared::event::Subscription sound_sub_;
  shared::event::Subscription load_sub_;

  bool is_paused_{ false };
};
}// namespace chroma::app::states
