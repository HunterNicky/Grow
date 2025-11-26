#pragma once

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Weapon.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/InputState.h"
#include "chroma/app/database/DatabaseTypes.h"

enum class FacingDir : uint8_t { Up = 0, Down = 1, Side = 2 };

namespace chroma::shared::core::player {
class Player final : public GameObject
{
public:
  Player();
  Player(const Player &) = default;
  Player(Player &&) = delete;
  Player &operator=(const Player &) = default;
  Player &operator=(Player &&) = delete;
  ~Player() override;
  void AnimationState(Vector2 dir, float magnitude);
  void AnimateAttack(const std::string &mode, FacingDir facing_dir) const;
  void AnimateMove(const std::string &mode, const std::string &state, Vector2 dir);

  void OnUpdate(float delta_time) override;
  void OnFixedUpdate(float fixed_delta_time) override;
  void OnCollision(const collision::CollisionEvent &event) override;
  void OnRender() override;

  void HandleEvent(event::Event &event) override;
  void HandleDirectionInput(Vector2 &direction) const;
  void HandleWeaponInput();
  void UpdateAttack(float delta_time) const;
  void HandleThrowInput(const std::shared_ptr<component::Weapon> &weapon) const;

  FacingDir GetLastFacingDir() const;

  void SetupAnimation(const std::shared_ptr<component::SpriteAnimation> &anim_component);
  std::shared_ptr<GameObject> Clone() override;

  void SetCurrentWeapon(const std::shared_ptr<component::Weapon> &weapon) const;

  void LoadPlayerWithPlayerData(const app::database::PlayerData &player_data);
  app::database::PlayerData SavePlayerToPlayerData() const;

  [[nodiscard]]bool IsLeft() const; 
  void SetIsLeft(bool is_left);
  [[nodiscard]]int GetCharacterSkin() const;

private:
  events::InputState input_state_;
  FacingDir last_facing_{ FacingDir::Down };
  bool last_left_{ false };
  float step_timer_{ 0.0F };
  bool was_moving_{ false };
};
}// namespace chroma::shared::core::player
