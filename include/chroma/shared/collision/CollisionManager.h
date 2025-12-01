#pragma once

#include "CollisionEvent.h"
#include "chroma/shared/collision/Quadtree.h"
#include "chroma/shared/context/GameContext.h"

#include <memory>
#include <raylib.h>
#include <utility>
#include <vector>

namespace chroma::shared::collision {
enum class BodyType : uint8_t { Static = 0, Dynamic = 1, All = 2 };

enum class CollisionResolutionStrategy : uint8_t { None = 0, ResolveA = 1, ResolveB = 2, ResolveBoth = 3 };

struct CollisionResponseConfig
{
  bool can_push{ true };
  bool can_be_pushed{ true };
  bool blocks_movement{ true };

  CollisionResponseConfig() = default;
  CollisionResponseConfig(const bool can_push, const bool can_be_pushed, const bool blocks = true)
    : can_push(can_push), can_be_pushed(can_be_pushed), blocks_movement(blocks)
  {}
};

class CollisionManager
{
public:
  explicit CollisionManager(Rectangle map_bounds, GameContextType context_type);
  ~CollisionManager() = default;

  void AddCollider(const ColliderEntry &collider, BodyType type);
  void RemoveCollider(ColliderEntry collider);

  [[nodiscard]] std::vector<UUIDv4::UUID> GetCollisions(const ColliderEntry &collider_box,
    BodyType type = BodyType::All) const;

  void SetContextType(GameContextType context_type);

  void Update() const;

private:
  Rectangle map_bounds_;
  GameContextType context_type_{ GameContextType::Client };

  std::unique_ptr<Quadtree> static_quadtree_;
  std::unique_ptr<Quadtree> dynamic_quadtree_;

  std::vector<ColliderEntry> static_colliders_;
  std::vector<ColliderEntry> dynamic_colliders_;

  void CheckCollision(const ColliderEntry &a, const ColliderEntry &b) const;

  [[nodiscard]] CollisionResolutionStrategy DetermineResolutionStrategy(const std::shared_ptr<core::GameObject> &obj_a,
    const std::shared_ptr<core::GameObject> &obj_b,
    BodyType type_a,
    BodyType type_b) const;

  static CollisionResolutionStrategy DetermineServerStrategy(const CollisionResponseConfig &response_a,
    const CollisionResponseConfig &response_b);
  static CollisionResolutionStrategy DetermineClientStrategy(const CollisionResponseConfig &response_a,
    const CollisionResponseConfig &response_b,
    core::NetRole role_a,
    core::NetRole role_b);

  struct OverlapInfo
  {
    Vector2 normal;
    float penetration;
    Vector2 contact;
  };

  static OverlapInfo CalculateOverlap(const Rectangle &bounds_a, const Rectangle &bounds_b);
  [[nodiscard]] std::pair<BodyType, BodyType> DetermineBodyTypes(const ColliderEntry &a, const ColliderEntry &b) const;
  static std::pair<CollisionEvent::Type, CollisionEvent::Type> DetermineEventTypes(const ColliderEntry &a,
    const ColliderEntry &b);

  void ResolveCollisionOneWay(const ColliderEntry &moving_obj, const CollisionEvent &event) const;

  void ResolveBothEqual(const ColliderEntry &obj_a,
    const ColliderEntry &obj_b,
    const Vector2 &normal,
    float penetration) const;

  void RebuildStaticTree() const;
};

}// namespace chroma::shared::collision