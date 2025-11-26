#pragma once
#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::core::enemy {
class Enemy final : public GameObject
{
public:
  Enemy();
  ~Enemy() override;

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  std::shared_ptr<GameObject> Clone() override;
  void OnFixedUpdate(float fixed_delta_time) override;
  void OnCollision(const collision::CollisionEvent &collision) override;
  void HandleEvent(event::Event &event) override;
};
}// namespace chroma::shared::core::enemy