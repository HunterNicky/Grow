#pragma once

#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::core::projectile {

class Projectile : public GameObject
{
public:
  Projectile();
  ~Projectile() override;

  Projectile(const Projectile &) = default;
  Projectile(Projectile &&) = delete;
  Projectile &operator=(const Projectile &) = default;
  Projectile &operator=(Projectile &&) = delete;

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  std::shared_ptr<GameObject> Clone() override;
  void OnFixedUpdate(float delta_time) override;
  void OnCollision(const collision::CollisionEvent &event) override;
  void HandleEvent(event::Event &event) override;


  void InitAnimation();
  void UpdateAnimation();
};
}// namespace chroma::shared::core::projectile