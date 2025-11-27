#pragma once
#include "chroma/shared/core/GameObject.h"

enum class EnemyFacingDir : uint8_t { Up = 0, Down = 1, Side = 2 };

namespace chroma::shared::core::enemy {
class Enemy final : public GameObject
{
public:
  Enemy();
  ~Enemy() override;
  void SetupAnimation() const;

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  std::shared_ptr<GameObject> Clone() override;
  void OnFixedUpdate(float fixed_delta_time) override;
  void OnCollision(const collision::CollisionEvent &collision) override;
  void HandleEvent(event::Event &event) override;

private:
  void UpdateAnimations(const Vector2 &dir, float magnitude);
  void UpdateRangedAttack(float delta_time, const Vector2 &dir);

  EnemyFacingDir last_facing_{ EnemyFacingDir::Down };
  bool last_left_{ false };
  float attack_cooldown_{ 1.2F };
  float last_attack_time_{ 0.0F };
};
}// namespace chroma::shared::core::enemy