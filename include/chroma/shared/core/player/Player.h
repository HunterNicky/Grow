#pragma once

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/InputState.h"

namespace chroma::shared::core::player {
class Player : public GameObject
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
  void HandleEvent(const shared::event::Event &event);
  std::shared_ptr<GameObject> Clone() override;

private:
  shared::events::InputState input_state_;
};
}// namespace chroma::shared::core::player
