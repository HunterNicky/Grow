#pragma once

#include "EnemyBlackboard.h"
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
    if (bb.owner.expired() || bb.target.expired()) return execution_state::failure;

    const auto owner = bb.owner.lock();
    const auto target = bb.target.lock();

    const auto transform = owner->GetComponent<core::component::Transform>();
    const auto movement = owner->GetComponent<core::component::Movement>();

    if (!transform || !movement) return execution_state::failure;

    const Vector2 target_real_pos = target->GetComponent<core::component::Transform>()->GetPosition();
    bb.target_position = target_real_pos;

    Vector2 direction = Vector2Subtract(bb.target_position, transform->GetPosition());
    const float dist = Vector2Length(direction);

    if (dist <= bb.attack_range) {
      movement->SetDirection({ 0.0f, 0.0f });
      return execution_state::success;
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
      if (const auto move = owner->GetComponent<core::component::Movement>()) { move->SetDirection({ 0.0f, 0.0f }); }
    }
    return execution_state::success;
  }
};

}// namespace chroma::shared::ai