#pragma once

#include "CollisionEvent.h"
#include "chroma/shared/collision/Quadtree.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/components/ColliderBox.h"

#include <memory>
#include <raylib.h>
#include <vector>

namespace chroma::shared::collision {
enum class BodyType : uint8_t { Static = 0, Dynamic = 1, All };

enum class CollisionResolutionStrategy : uint8_t { None = 0, ResolveA = 1, ResolveB = 2, ResolveBoth = 3 };

struct CollisionResponseConfig
{
  bool can_push{ true };
  bool can_be_pushed{ true };
  bool blocks_movement{ true };

  CollisionResponseConfig() = default;
  CollisionResponseConfig(bool can_push, bool can_be_pushed, bool blocks = true)
    : can_push(can_push), can_be_pushed(can_be_pushed), blocks_movement(blocks)
  {}
};

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

  [[nodiscard]] CollisionResolutionStrategy DetermineResolutionStrategy(const std::shared_ptr<core::GameObject> &obj_a,
    const std::shared_ptr<core::GameObject> &obj_b,
    BodyType type_a,
    BodyType type_b) const;

  static void ResolveCollisionOneWay(const std::shared_ptr<core::component::ColliderBox> &moving_obj,
    const CollisionEvent &event);

  static void ResolveBothEqual(const std::shared_ptr<core::component::ColliderBox> &obj_a,
    const std::shared_ptr<core::component::ColliderBox> &obj_b,
    const Vector2 &normal,
    float penetration);

  void RebuildStaticTree() const;
};

}// namespace chroma::shared::collision