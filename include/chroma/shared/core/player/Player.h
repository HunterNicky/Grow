#pragma once

#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::core::player {
class Player : public GameObject
{
public:
  Player();
  Player(const Player &) = default;
  Player(Player &&) = delete;
  Player &operator=(const Player &) = default;
  Player &operator=(Player &&) = delete;

  void InitComponents();
  ~Player() override;
  void OnUpdate(float delta_time) override;
  void OnFixedUpdate(float fixed_delta_time) override;
  void OnCollision(const collision::CollisionEvent &event) override;
  void OnRender() override;
};
}// namespace chroma::shared::core::player
