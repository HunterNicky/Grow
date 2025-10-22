#pragma once

#include <cstdint>
#include <memory>
#include <raylib.h>
#include <utility>

#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::collision {
enum class CollisionSide : uint8_t { None = 0, Top = 1, Bottom = 2, Left = 3, Right = 4 };

struct CollisionEvent
{
  std::weak_ptr<core::GameObject> other;

  CollisionSide side;
  float penetration;

  Vector2 normal;
  Vector2 contact_point;

  CollisionEvent(std::weak_ptr<core::GameObject> other,
    float penetration,
    Vector2 normal,
    Vector2 contact_point,
    CollisionSide side = CollisionSide::None)
    : other(std::move(other)), side(side), penetration(penetration), normal(normal), contact_point(contact_point)
  {
    if (side == CollisionSide::None) {
      // talvez tenha que ser o valor de 1 ou -1, vai depender do sistema de coordenadas/implementação
      if (normal.y > 0.F) {
        this->side = CollisionSide::Top;
      } else if (normal.y < 0.F) {
        this->side = CollisionSide::Bottom;
      } else if (normal.x < 0.F) {
        this->side = CollisionSide::Left;
      } else if (normal.x > 0.F) {
        this->side = CollisionSide::Right;
      }
    }
  }
};
}// namespace chroma::shared::collision
