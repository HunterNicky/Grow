#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Health.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <raymath.h>
#include <unordered_map>
#include <uuid_v4.h>

namespace chroma::app::states::network {


void InterpolateSystem::Interpolate(
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &new_snapshot,
  uint64_t delta_time)
{
  if (past_game_objects_.empty()) {
    past_game_objects_ = new_snapshot;
    target_game_objects_ = new_snapshot;
    interp_elapsed_ = 0;
    time_last_snapshot_ = delta_time;
    return;
  }

  if (!new_snapshot.empty() && new_snapshot != target_game_objects_) {
    past_game_objects_ = target_game_objects_;
    target_game_objects_ = new_snapshot;
    interp_elapsed_ = 0;

    const uint64_t interval = delta_time - time_last_snapshot_;

    snapshot_interval_ = interval > 0 ? interval : snapshot_interval_;
    time_last_snapshot_ = delta_time;
  }
}

void InterpolateSystem::InterpolatePosition(const std::shared_ptr<shared::core::GameObject> &past_object,
  const std::shared_ptr<shared::core::GameObject> &target_object,
  std::shared_ptr<shared::core::GameObject> &out_object,
  float alpha) const
{
  if (out_object->GetId() == player_id_) { return; }

  const auto past_pos = past_object->GetTransform()->GetPosition();
  const auto target_pos = target_object->GetTransform()->GetPosition();
  const auto interpolated_pos = Vector2Lerp(past_pos, target_pos, alpha);
  out_object->GetTransform()->SetPosition(interpolated_pos);
}

void InterpolateSystem::InterpolateHealth(const std::shared_ptr<shared::core::GameObject> &past_object,
  const std::shared_ptr<shared::core::GameObject> &target_object,
  std::shared_ptr<shared::core::GameObject> &out_object,
  float alpha)
{
  const auto past_health = past_object->GetComponent<shared::core::component::Health>();
  const auto target_health = target_object->GetComponent<shared::core::component::Health>();
  const auto out_health = out_object->GetComponent<shared::core::component::Health>();

  if (past_health && target_health && out_health) {
    const float past_value = *past_health->GetCurrentHealth();
    const float target_value = *target_health->GetCurrentHealth();
    const float interpolated_value = past_value + ((target_value - past_value) * alpha);
    out_health->SetCurrentHealth(interpolated_value);
  }
}

uint64_t InterpolateSystem::GetTimeLastSnapshot() const { return time_last_snapshot_; }

void InterpolateSystem::SetSnapshotInterval(uint64_t interval) { snapshot_interval_ = interval; }

void InterpolateSystem::Update(uint64_t delta_time)
{
  if (!game_objects_ || past_game_objects_.empty() || target_game_objects_.empty()) { return; }

  interp_elapsed_ += delta_time;
  float alpha = static_cast<float>(interp_elapsed_) / static_cast<float>(snapshot_interval_);
  alpha = std::clamp(alpha, 0.0F, 1.0F);

  for (auto &[id, target_obj] : target_game_objects_) {
    if (!target_obj) { continue; }

    if (id == player_id_) { continue; }

    auto past_it = past_game_objects_.find(id);
    if (past_it != past_game_objects_.end() && past_it->second) {
      auto interpolated = target_obj->Clone();
      if (!interpolated) { continue; }
      interpolated->SetNetRole(target_obj->GetNetRole());
      InterpolatePosition(past_it->second, target_obj, interpolated, alpha);
      InterpolateHealth(past_it->second, target_obj, interpolated, alpha);
      (*game_objects_)[id] = interpolated;
    } else {
      const auto clone = target_obj->Clone();
      if (!clone) { continue; }
      clone->SetNetRole(target_obj->GetNetRole());
      (*game_objects_)[id] = clone;
    }
  }
}

void InterpolateSystem::SetGameObjects(
  const std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>> &objects)
{
  game_objects_ = objects;
}

void InterpolateSystem::SetPlayerId(const UUIDv4::UUID &player_id) { player_id_ = player_id; }

}// namespace chroma::app::states::network
