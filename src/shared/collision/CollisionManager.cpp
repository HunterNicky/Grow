#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/Transform.h"

#include <algorithm>
#include <raymath.h>
#include <unordered_set>

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

  const CollisionEvent event(b->GetGameObject(), penetration, normal, contact);

  if (context_type_ == GameContextType::Client) {
    const auto obj_a = a->GetGameObject();
    const auto obj_b = b->GetGameObject();

    const bool a_is_local = obj_a->GetNetRole() == core::NetRole::AUTONOMOUS;
    const bool b_is_local = obj_b->GetNetRole() == core::NetRole::AUTONOMOUS;

    if (a_is_local && !b_is_local) {
      ResolveCollisionAsStatic(a, event);
      return;
    }

    if (b_is_local && !a_is_local) {
      ResolveCollisionAsStatic(b, event);
      return;
    }
  }

  ResolveCollision(a, event);
}

void CollisionManager::ResolveCollision(const std::shared_ptr<core::component::ColliderBox> &dynamic_obj,
  const CollisionEvent &event)
{
  if (const auto owner = dynamic_obj->GetGameObject()) {
    if (const auto transform = owner->GetComponent<core::component::Transform>()) {
      Vector2 current_pos = transform->GetPosition();

      const Vector2 correction = { event.normal.x * event.penetration, event.normal.y * event.penetration };

      current_pos = Vector2Subtract(current_pos, correction);

      transform->SetPosition(current_pos);

      if (const auto velocity = owner->GetComponent<core::component::Speed>()) {
        Vector2 vel = velocity->GetSpeed();
        const float dot = Vector2DotProduct(vel, event.normal);

        if (dot < 0) {
          const Vector2 vel_correction = { event.normal.x * dot, event.normal.y * dot };
          vel = Vector2Subtract(vel, vel_correction);

          velocity->SetSpeed(vel);
        }
      }

      dynamic_obj->Update(0.0F);
    }
  }
}

void CollisionManager::ResolveCollisionAsStatic(const std::shared_ptr<core::component::ColliderBox> &dynamic_obj,
  const CollisionEvent &event)
{
  if (const auto owner = dynamic_obj->GetGameObject()) {
    if (const auto transform = owner->GetComponent<core::component::Transform>()) {
      Vector2 current_pos = transform->GetPosition();
      const Vector2 correction = { event.normal.x * event.penetration, event.normal.y * event.penetration };

      current_pos = Vector2Subtract(current_pos, correction);
      transform->SetPosition(current_pos);
    }

    if (const auto speed_comp = owner->GetComponent<core::component::Speed>()) {
      Vector2 velocity = speed_comp->GetSpeed();
      const float dot = Vector2DotProduct(velocity, event.normal);
      if (dot > 0) {
        const Vector2 wall_component = { event.normal.x * dot, event.normal.y * dot };
        velocity = Vector2Subtract(velocity, wall_component);

        speed_comp->SetSpeed(velocity);
      }
    }
    dynamic_obj->Update(0.0F);
  }
}

}// namespace chroma::shared::collision