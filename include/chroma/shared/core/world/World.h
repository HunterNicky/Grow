#pragma once
#include "WorldSystem.h"
#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::core::world {
class World final : public GameObject
{
public:
  World();
  World(const World &) = delete;
  World &operator=(const World &) = delete;
  World(World &) = default;
  World(World &&) = delete;
  World &operator=(World &&) = delete;

  ~World() override;

  void OnRender() override;
  void OnUpdate(float delta_time) override;
  std::shared_ptr<GameObject> Clone() override;
  void OnFixedUpdate(float fixed_delta_time) override;
  void OnCollision(const collision::CollisionEvent &collision) override;
  void HandleEvent(event::Event &event) override;

private:
};
}// namespace chroma::shared::core::world