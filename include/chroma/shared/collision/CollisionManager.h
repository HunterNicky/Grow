#pragma once

#include "CollisionEvent.h"
#include "chroma/shared/collision/Quadtree.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/components/ColliderBox.h"

#include <memory>
#include <raylib.h>
#include <vector>

namespace chroma::shared::collision {
enum class BodyType : uint8_t { Static, Dynamic, All };

class CollisionManager
{
public:
  explicit CollisionManager(Rectangle map_bounds);
  ~CollisionManager() = default;

  void AddCollider(const std::shared_ptr<core::component::ColliderBox> &collider, BodyType type);
  void RemoveCollider(const std::shared_ptr<core::component::ColliderBox> &collider);

  std::vector<std::shared_ptr<core::GameObject>> GetCollisions(
    const std::shared_ptr<core::component::ColliderBox> &collider_box,
    BodyType type = BodyType::All) const;

  void SetContextType(GameContextType context_type);

  void Update() const;

private:
  Rectangle map_bounds_;
  GameContextType context_type_{ GameContextType::Client };

  std::unique_ptr<Quadtree> static_quadtree_;
  std::unique_ptr<Quadtree> dynamic_quadtree_;

  std::vector<std::shared_ptr<core::component::ColliderBox>> static_colliders_;
  std::vector<std::shared_ptr<core::component::ColliderBox>> dynamic_colliders_;

  void CheckCollision(const std::shared_ptr<core::component::ColliderBox> &a,
    const std::shared_ptr<core::component::ColliderBox> &b) const;
  static void ResolveCollision(const std::shared_ptr<core::component::ColliderBox> &dynamic_obj,
    const CollisionEvent &event);
  static void ResolveCollisionAsStatic(const std::shared_ptr<core::component::ColliderBox> &dynamic_obj,
                                const CollisionEvent &event);

  void RebuildStaticTree() const;
};

}// namespace chroma::shared::collision