#pragma once

#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::collision {

class CollisionResponseResolver
{
public:
  CollisionResponseResolver() = default;
  ~CollisionResponseResolver() = default;

  [[nodiscard]] static CollisionResponseConfig Resolve(const std::shared_ptr<core::GameObject> &obj,
    BodyType body_type);
};

}// namespace chroma::shared::collision
