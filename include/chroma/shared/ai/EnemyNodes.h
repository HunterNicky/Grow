#pragma once

#include "EnemyBlackboard.h"
#include "chroma/shared/ai/Astar.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Transform.h"
#include <aitoolkit/behtree.hpp>
#include <raymath.h>

namespace chroma::shared::ai {

using namespace aitoolkit::bt;

struct CheckHasTarget
{
  bool operator()(const EnemyBlackboard &bb) const
  {
    if (bb.target.expired() || bb.owner.expired()) { return false; }

    const auto owner = bb.owner.lock();
    const auto target = bb.target.lock();

    const auto owner_pos = owner->GetComponent<core::component::Transform>()->GetPosition();
    const auto target_pos = target->GetComponent<core::component::Transform>()->GetPosition();

    const float dist = Vector2Distance(owner_pos, target_pos);

    return dist <= bb.sight_range;
  }
};

struct TaskChaseTarget
{
  execution_state operator()(EnemyBlackboard &bb) const
  {
    if (bb.owner.expired() || bb.target.expired()) { return execution_state::failure; }
    const auto owner = bb.owner.lock();
    const auto target = bb.target.lock();

    const auto transform = owner->GetComponent<core::component::Transform>();
    const auto movement = owner->GetComponent<core::component::Movement>();

    if (!transform || !movement) { return execution_state::failure; }
    const Vector2 target_real_pos = target->GetComponent<core::component::Transform>()->GetPosition();
    bb.target_position = target_real_pos;

    if (!bb.nav_grid || bb.grid_width <= 0 || bb.grid_height <= 0 || bb.tile_size <= 0) {
      Vector2 direction = Vector2Subtract(bb.target_position, transform->GetPosition());
      const float dist = Vector2Length(direction);

      if (dist <= bb.attack_range) {
        movement->SetDirection({ 0.0F, 0.0F });
        return execution_state::success;
      }

      direction = Vector2Normalize(direction);
      movement->SetDirection(direction);
      return execution_state::running;
    }

    const Vector2 owner_pos = transform->GetPosition();
    const Vector2 start_grid = { owner_pos.x / static_cast<float>(bb.tile_size),
      owner_pos.y / static_cast<float>(bb.tile_size) };
    const Vector2 end_grid = { target_real_pos.x / static_cast<float>(bb.tile_size),
      target_real_pos.y / static_cast<float>(bb.tile_size) };

    const float direct_dist = Vector2Distance(owner_pos, target_real_pos);
    if (direct_dist <= bb.attack_range) {
      movement->SetDirection({ 0.0F, 0.0F });
      return execution_state::success;
    }

    if (bb.current_path.empty() || bb.path_index >= bb.current_path.size()) {
      bb.current_path = Astar::Solve(start_grid, end_grid, *bb.nav_grid, bb.grid_width, bb.grid_height);
      bb.path_index = 0;
    }

    if (bb.current_path.empty() || bb.path_index >= bb.current_path.size()) {
      Vector2 direction = Vector2Subtract(bb.target_position, owner_pos);
      if (Vector2Length(direction) > 0.0F) {
        direction = Vector2Normalize(direction);
        movement->SetDirection(direction);
        return execution_state::running;
      }
      movement->SetDirection({ 0.0F, 0.0F });
      return execution_state::failure;
    }

    const Vector2 next_node = {
      (bb.current_path[bb.path_index].x * static_cast<float>(bb.tile_size))
        + (static_cast<float>(bb.tile_size) * 0.5F),
      (bb.current_path[bb.path_index].y * static_cast<float>(bb.tile_size))
        + (static_cast<float>(bb.tile_size) * 0.5F)
    };

    Vector2 direction = Vector2Subtract(next_node, owner_pos);
    const float dist_to_next = Vector2Length(direction);

    if (dist_to_next < 1.0F) {
      ++bb.path_index;
      if (bb.path_index >= bb.current_path.size()) {
        return execution_state::success;
      }
      return execution_state::running;
    }

    direction = Vector2Normalize(direction);
    movement->SetDirection(direction);

    return execution_state::running;
  }
};

struct TaskIdle
{
  execution_state operator()(const EnemyBlackboard &bb) const
  {
    if (const auto owner = bb.owner.lock()) {
      if (const auto move = owner->GetComponent<core::component::Movement>()) { move->SetDirection({ 0.0F, 0.0F }); }
    }
    return execution_state::success;
  }
};

}// namespace chroma::shared::ai