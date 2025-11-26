#pragma once
#include "chroma/shared/core/GameObject.h"
#include <memory>
#include <raylib.h>

namespace chroma::shared::ai {

struct EnemyBlackboard
{
  std::weak_ptr<core::GameObject> owner;
  std::weak_ptr<core::GameObject> target;

  Vector2 target_position = { 0.0F, 0.0F };
  float attack_range = 30.0F;
  float sight_range = 300.0F;
};
}// namespace chroma::shared::ai