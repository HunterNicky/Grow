#include "chroma/shared/collision/CollisionResponseResolver.h"
#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"

#include <memory>

namespace chroma::shared::collision {

CollisionResponseConfig CollisionResponseResolver::Resolve(const std::shared_ptr<core::GameObject> &obj,
  const BodyType body_type)
{
  if (!obj) { return { false, false, false }; }

  if (body_type == BodyType::Static) { return { false, false, true }; }

  CollisionResponseConfig config(true, true, true);

  const core::GameObjectType obj_type = obj->GetTag();
  switch (obj_type) {
  case core::GameObjectType::PLAYER:
  case core::GameObjectType::ENEMY:
    config.can_push = true;
    config.can_be_pushed = true;
    config.blocks_movement = true;
    break;
  case core::GameObjectType::PROJECTILE:
    config.can_push = false;
    config.can_be_pushed = false;
    config.blocks_movement = false;
    break;
  case core::GameObjectType::OBSTACLE:
    config.can_push = false;
    config.can_be_pushed = true;
    config.blocks_movement = true;
    break;
  case core::GameObjectType::WORLD:
    config.can_push = false;
    config.can_be_pushed = false;
    config.blocks_movement = true;
  default:
    break;
  }

  return config;
}

}// namespace chroma::shared::collision
