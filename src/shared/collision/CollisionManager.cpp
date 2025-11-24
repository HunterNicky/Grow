#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/collision/CollisionEvent.h"
#include "chroma/shared/collision/Quadtree.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/ColliderBox.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/collision/CollisionResponseResolver.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
#include <vector>

namespace chroma::shared::collision {
CollisionManager::CollisionManager(const Rectangle map_bounds) : map_bounds_(map_bounds)
{
  static_quadtree_ = std::make_unique<Quadtree>(0, map_bounds_);
  dynamic_quadtree_ = std::make_unique<Quadtree>(0, map_bounds_);
}

void CollisionManager::AddCollider(const std::shared_ptr<core::component::ColliderBox> &collider, const BodyType type)
{
  if (!collider) { return; }

  auto contains = [&](auto &vec) { return std::ranges::find(vec, collider) != vec.end(); };

  if (type == BodyType::Static) {
    if (!contains(static_colliders_)) {
      static_colliders_.push_back(collider);
      if (collider->GetGameObject()) { static_quadtree_->Insert(collider); }
    }
  } else {
    if (!contains(dynamic_colliders_)) { dynamic_colliders_.push_back(collider); }
  }
}

void CollisionManager::RemoveCollider(const std::shared_ptr<core::component::ColliderBox> &collider)
{
  if (!collider) { return; }

  auto eraseFrom = [&](auto &vec) {
    auto sub = std::ranges::remove(vec, collider);
    auto new_end = sub.begin();
    if (new_end != vec.end()) {
      vec.erase(new_end, vec.end());
      return true;
    }
    return false;
  };

  if (eraseFrom(dynamic_colliders_)) { return; }
  if (eraseFrom(static_colliders_)) { RebuildStaticTree(); }
}
std::vector<std::shared_ptr<core::GameObject>> CollisionManager::GetCollisions(
  const std::shared_ptr<core::component::ColliderBox> &collider_box,
  const BodyType type) const
{
  if (!collider_box) { return {}; }

  std::vector<std::shared_ptr<core::GameObject>> collisions;
  std::unordered_set<const core::GameObject *> seen;
  if (type == BodyType::Static || type == BodyType::All) {
    std::vector<std::shared_ptr<core::component::ColliderBox>> return_objects = {};
    static_quadtree_->Retrieve(return_objects, collider_box);
    for (const auto &target : return_objects) {
      if (target == collider_box) { continue; }
      auto game_object = target->GetGameObject();
      if (game_object && seen.insert(game_object.get()).second) { collisions.push_back(game_object); }
    }
  }

  if (type == BodyType::Dynamic || type == BodyType::All) {
    std::vector<std::shared_ptr<core::component::ColliderBox>> return_objects = {};
    dynamic_quadtree_->Retrieve(return_objects, collider_box);
    for (const auto &target : return_objects) {
      if (target == collider_box) { continue; }
      auto game_object = target->GetGameObject();
      if (game_object && seen.insert(game_object.get()).second) { collisions.push_back(game_object); }
    }
  }

  return collisions;
}

void CollisionManager::SetContextType(const GameContextType context_type) { context_type_ = context_type; }

void CollisionManager::RebuildStaticTree() const
{
  static_quadtree_->Clear();
  for (const auto &collider : static_colliders_) {
    if (collider->GetGameObject()) { static_quadtree_->Insert(collider); }
  }
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

  for (const auto &collider : dynamic_colliders_) {
    if (collider->GetGameObject()) { dynamic_quadtree_->Insert(collider); }
  }

  for (const auto &dynamic_obj : dynamic_colliders_) {
    if (!dynamic_obj->GetGameObject()) { continue; }

    std::vector<std::shared_ptr<core::component::ColliderBox>> return_objects = {};
    static_quadtree_->Retrieve(return_objects, dynamic_obj);
    for (auto &target : return_objects) { CheckCollision(dynamic_obj, target); }

    dynamic_quadtree_->Retrieve(return_objects, dynamic_obj);
    for (auto &target : return_objects) {
      if (dynamic_obj == target) { continue; }
      if (!target->GetGameObject()) { continue; }

      CheckCollision(dynamic_obj, target);
    }
  }
}

void CollisionManager::CheckCollision(const std::shared_ptr<core::component::ColliderBox> &a,
  const std::shared_ptr<core::component::ColliderBox> &b) const
{
  const Rectangle bounds_a = a->GetBoundingBox();
  const Rectangle bounds_b = b->GetBoundingBox();

  if (!CheckCollisionRecs(bounds_a, bounds_b)) { return; }

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

  const auto obj_a = a->GetGameObject();
  const auto obj_b = b->GetGameObject();
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

void CollisionManager::ResolveCollisionOneWay(const std::shared_ptr<core::component::ColliderBox> &moving_obj,
  const CollisionEvent &event) const
{
  if (!moving_obj) { return; }

  const auto owner = moving_obj->GetGameObject();
  if (!owner) { return; }

  const auto transform = owner->GetComponent<core::component::Transform>();
  if (!transform) { return; }

  Vector2 current_pos = transform->GetPosition();
  const Vector2 correction = { event.normal.x * event.penetration, event.normal.y * event.penetration };
  current_pos = Vector2Subtract(current_pos, correction);
  transform->SetPosition(current_pos);

  moving_obj->Update(0.0F);
}

void CollisionManager::ResolveBothEqual(const std::shared_ptr<core::component::ColliderBox> &obj_a,
  const std::shared_ptr<core::component::ColliderBox> &obj_b,
  const Vector2 &normal,
  const float penetration) const
{
  if (!obj_a || !obj_b) { return; }

  const auto owner_a = obj_a->GetGameObject();
  const auto owner_b = obj_b->GetGameObject();
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

  obj_a->Update(0.0F);
  obj_b->Update(0.0F);
}

}// namespace chroma::shared::collision