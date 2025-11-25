#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/collision/CollisionEvent.h"
#include "chroma/shared/collision/CollisionResponseResolver.h"
#include "chroma/shared/collision/Quadtree.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/Transform.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
#include <uuid_v4.h>
#include <vector>

namespace chroma::shared::collision {
CollisionManager::CollisionManager(const Rectangle map_bounds, const GameContextType context_type)
  : map_bounds_(map_bounds), context_type_(context_type)
{
  static_quadtree_ = std::make_unique<Quadtree>(0, map_bounds_);
  dynamic_quadtree_ = std::make_unique<Quadtree>(0, map_bounds_);
}

void CollisionManager::AddCollider(const ColliderEntry &collider, const BodyType type)
{
  if (type == BodyType::Static) {
    auto it = std::ranges::find(static_colliders_, collider);
    if (it == static_colliders_.end()) {
      static_colliders_.push_back(collider);
      static_quadtree_->Insert(collider);
    }
  } else {
    auto it = std::ranges::find(dynamic_colliders_, collider);
    if (it == dynamic_colliders_.end()) { dynamic_colliders_.push_back(collider); }
  }
}

void CollisionManager::RemoveCollider(ColliderEntry collider)
{
  auto erase_from = [&](auto &vec) {
    auto sub = std::ranges::remove(vec, collider);
    auto new_end = sub.begin();
    if (new_end != vec.end()) {
      vec.erase(new_end, vec.end());
      return true;
    }
    return false;
  };

  if (erase_from(dynamic_colliders_)) { return; }
  if (erase_from(static_colliders_)) { RebuildStaticTree(); }
}

std::vector<UUIDv4::UUID> CollisionManager::GetCollisions(const ColliderEntry &collider_box, const BodyType type) const
{
  std::vector<UUIDv4::UUID> collisions;
  std::unordered_set<UUIDv4::UUID> seen;
  if (type == BodyType::Static || type == BodyType::All) {
    std::vector<ColliderEntry> return_objects = {};
    static_quadtree_->Retrieve(return_objects, collider_box);

    for (const auto &target : return_objects) {
      if (target == collider_box) { continue; }
      if (seen.insert(target.id).second) { collisions.push_back(target.id); }
    }
  }

  if (type == BodyType::Dynamic || type == BodyType::All) {
    std::vector<ColliderEntry> return_objects = {};
    dynamic_quadtree_->Retrieve(return_objects, collider_box);
    for (const auto &target : return_objects) {
      if (target == collider_box) { continue; }
      if (seen.insert(target.id).second) { collisions.push_back(target.id); }
    }
  }

  return collisions;
}

void CollisionManager::SetContextType(const GameContextType context_type) { context_type_ = context_type; }

void CollisionManager::RebuildStaticTree() const
{
  static_quadtree_->Clear();
  for (const auto &collider : static_colliders_) { static_quadtree_->Insert(collider); }
}

CollisionResolutionStrategy CollisionManager::DetermineResolutionStrategy(
  const std::shared_ptr<core::GameObject> &obj_a,
  const std::shared_ptr<core::GameObject> &obj_b,
  const BodyType type_a,
  const BodyType type_b) const
{
  if (!obj_a || !obj_b) { return CollisionResolutionStrategy::None; }

  const core::NetRole role_a = obj_a->GetNetRole();
  const core::NetRole role_b = obj_b->GetNetRole();

  const auto response_a = CollisionResponseResolver::Resolve(obj_a, type_a);
  const auto response_b = CollisionResponseResolver::Resolve(obj_b, type_b);

  if (type_a == BodyType::Static && type_b == BodyType::Static) { return CollisionResolutionStrategy::None; }

  if (type_a == BodyType::Static) {
    return response_b.can_be_pushed ? CollisionResolutionStrategy::ResolveB : CollisionResolutionStrategy::None;
  }

  if (type_b == BodyType::Static) {
    return response_a.can_be_pushed ? CollisionResolutionStrategy::ResolveA : CollisionResolutionStrategy::None;
  }

  if (context_type_ == GameContextType::Server) {
    if (response_a.can_be_pushed && response_b.can_be_pushed) { return CollisionResolutionStrategy::ResolveBoth; }
    if (response_a.can_be_pushed) { return CollisionResolutionStrategy::ResolveA; }
    if (response_b.can_be_pushed) { return CollisionResolutionStrategy::ResolveB; }
    return CollisionResolutionStrategy::None;
  }

  if (context_type_ == GameContextType::Client) {
    const bool a_is_local = (role_a == core::NetRole::AUTONOMOUS);
    const bool b_is_local = (role_b == core::NetRole::AUTONOMOUS);

    if (a_is_local && b_is_local && response_a.can_be_pushed && response_b.can_be_pushed) {
      return CollisionResolutionStrategy::ResolveBoth;
    }

    if (a_is_local && !b_is_local) {
      return response_a.can_be_pushed && response_b.blocks_movement ? CollisionResolutionStrategy::ResolveA
                                                                    : CollisionResolutionStrategy::None;
    }

    if (b_is_local && !a_is_local) {
      return response_b.can_be_pushed && response_a.blocks_movement ? CollisionResolutionStrategy::ResolveB
                                                                    : CollisionResolutionStrategy::None;
    }

    return CollisionResolutionStrategy::None;
  }

  return CollisionResolutionStrategy::None;
}

void CollisionManager::Update() const
{
  dynamic_quadtree_->Clear();

  for (const auto &collider : dynamic_colliders_) { dynamic_quadtree_->Insert(collider); }

  for (const auto &dynamic_obj : dynamic_colliders_) {
    std::vector<ColliderEntry> return_objects = {};
    static_quadtree_->Retrieve(return_objects, dynamic_obj);
    for (auto &target : return_objects) { CheckCollision(dynamic_obj, target); }

    dynamic_quadtree_->Retrieve(return_objects, dynamic_obj);
    for (auto &target : return_objects) {
      if (dynamic_obj == target) { continue; }

      CheckCollision(dynamic_obj, target);
    }
  }
}

void CollisionManager::CheckCollision(const ColliderEntry &a, const ColliderEntry &b) const
{
  const Rectangle bounds_a = *a.bounds;
  const Rectangle bounds_b = *b.bounds;

  if (!CheckCollisionRecs(bounds_a, bounds_b)) { return; }

  if (context_type_ == GameContextType::Server) {
    a.component->Update(0.0F);
    b.component->Update(0.0F);
  }

  const Vector2 half_a = { bounds_a.width * 0.5F, bounds_a.height * 0.5F };
  const Vector2 half_b = { bounds_b.width * 0.5F, bounds_b.height * 0.5F };

  const Vector2 center_a = { bounds_a.x + half_a.x, bounds_a.y + half_a.y };
  const Vector2 center_b = { bounds_b.x + half_b.x, bounds_b.y + half_b.y };

  const Vector2 delta = Vector2Subtract(center_b, center_a);

  const float overlap_x = (half_a.x + half_b.x) - std::fabs(delta.x);
  const float overlap_y = (half_a.y + half_b.y) - std::fabs(delta.y);

  if (overlap_x <= 0.0F || overlap_y <= 0.0F) { return; }

  Vector2 normal = { 0.0F, 0.0F };
  float penetration = 0.0F;

  if (overlap_x < overlap_y) {
    normal.x = (delta.x > 0.0F) ? 1.0F : -1.0F;
    penetration = overlap_x;
  } else {
    normal.y = (delta.y > 0.0F) ? 1.0F : -1.0F;
    penetration = overlap_y;
  }

  const Vector2 half_pen = { penetration * 0.5F, penetration * 0.5F };
  const Vector2 offset = { normal.x * (half_a.x - half_pen.x), normal.y * (half_a.y - half_pen.y) };
  const Vector2 contact = Vector2Add(center_a, offset);

  const auto manager = GCM::Instance().GetContext(context_type_)->GetGameObjectManager();
  const auto obj_a = manager ? manager->Get(a.id) : nullptr;
  const auto obj_b = manager ? manager->Get(b.id) : nullptr;

  if (!obj_a || !obj_b) { return; }

  BodyType type_a = BodyType::Dynamic;
  BodyType type_b = BodyType::Dynamic;

  for (const auto &collider : static_colliders_) {
    if (collider == a) { type_a = BodyType::Static; }
    if (collider == b) { type_b = BodyType::Static; }
  }

  const CollisionResolutionStrategy strategy = DetermineResolutionStrategy(obj_a, obj_b, type_a, type_b);

  const CollisionEvent event_a(obj_b, penetration, normal, contact);
  const CollisionEvent event_b(obj_a, penetration, Vector2Negate(normal), contact);

  obj_a->OnCollision(event_a);
  obj_b->OnCollision(event_b);

  switch (strategy) {
  case CollisionResolutionStrategy::None:
    break;

  case CollisionResolutionStrategy::ResolveA: {
    ResolveCollisionOneWay(a, event_a);
    break;
  }

  case CollisionResolutionStrategy::ResolveB: {
    ResolveCollisionOneWay(b, event_b);
    break;
  }

  case CollisionResolutionStrategy::ResolveBoth: {
    ResolveBothEqual(a, b, normal, penetration);
    break;
  }
  }
}

void CollisionManager::ResolveCollisionOneWay(const ColliderEntry &moving_obj, const CollisionEvent &event) const
{
  const auto manager = GCM::Instance().GetContext(context_type_)->GetGameObjectManager();
  const auto owner = manager ? manager->Get(moving_obj.id) : nullptr;
  if (!owner) { return; }

  const auto transform = owner->GetComponent<core::component::Transform>();
  if (!transform) { return; }

  Vector2 current_pos = transform->GetPosition();
  const Vector2 correction = { event.normal.x * event.penetration, event.normal.y * event.penetration };
  current_pos = Vector2Subtract(current_pos, correction);
  transform->SetPosition(current_pos);

  moving_obj.component->Update(0.0F);
}

void CollisionManager::ResolveBothEqual(const ColliderEntry &obj_a,
  const ColliderEntry &obj_b,
  const Vector2 &normal,
  const float penetration) const
{
  const auto manager = GCM::Instance().GetContext(context_type_)->GetGameObjectManager();
  const auto owner_a = manager ? manager->Get(obj_a.id) : nullptr;
  const auto owner_b = manager ? manager->Get(obj_b.id) : nullptr;
  if (!owner_a || !owner_b) { return; }

  const auto transform_a = owner_a->GetComponent<core::component::Transform>();
  const auto transform_b = owner_b->GetComponent<core::component::Transform>();
  if (!transform_a || !transform_b) { return; }

  const float half_penetration = penetration * 0.5F;
  const Vector2 correction = { normal.x * half_penetration, normal.y * half_penetration };

  Vector2 pos_a = transform_a->GetPosition();
  pos_a = Vector2Subtract(pos_a, correction);
  transform_a->SetPosition(pos_a);

  Vector2 pos_b = transform_b->GetPosition();
  pos_b = Vector2Add(pos_b, correction);
  transform_b->SetPosition(pos_b);

  obj_a.component->Update(0.0F);
  obj_b.component->Update(0.0F);
}

}// namespace chroma::shared::collision