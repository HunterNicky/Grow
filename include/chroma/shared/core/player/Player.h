#pragma once

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/InputState.h"
 
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

  void InitComponents();
  void OnUpdate(float delta_time) override;
  void OnFixedUpdate(float fixed_delta_time) override;
  void OnCollision(const collision::CollisionEvent &event) override;
  void OnRender() override;
  void HandleEvent(const event::Event &event);

  void UpdateAnimationFromDirection(Vector2 dir);

  static void SetupAnimation(const std::shared_ptr<component::SpriteAnimation> &anim_component);
  std::shared_ptr<GameObject> Clone() override;

private:
  events::InputState input_state_;
  FacingDir last_facing_ { FacingDir::Down };
  bool last_left_ { false };
  float step_timer_ { 0.0F };
  bool was_moving_ { false };
};
}// namespace chroma::shared::core::player
