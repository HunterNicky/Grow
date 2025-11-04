#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/shared/core/GameObject.h"

#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <raymath.h>
#include <uuid_v4.h>

namespace chroma::app::states::network {


void InterpolateSystem::Interpolate(
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> &new_snapshot,
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

  Update(delta_time);
}

void InterpolateSystem::InterpolatePosition(const std::shared_ptr<chroma::shared::core::GameObject> &past_object,
  const std::shared_ptr<chroma::shared::core::GameObject> &target_object,
  std::shared_ptr<chroma::shared::core::GameObject> &out_object,
  float alpha)
{
  if (out_object->GetId() == player_id_) { return; }

  const auto past_pos = past_object->GetTransform()->GetPosition();
  const auto target_pos = target_object->GetTransform()->GetPosition();
  const auto interpolated_pos = Vector2Lerp(past_pos, target_pos, alpha);
  out_object->GetTransform()->SetPosition(interpolated_pos);
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
    auto past_it = past_game_objects_.find(id);
    if (past_it != past_game_objects_.end()) {
      auto interpolated = target_obj->Clone();
      InterpolatePosition(past_it->second, target_obj, interpolated, alpha);
      (*game_objects_)[id] = interpolated;
    } else {
      (*game_objects_)[id] = target_obj;
    }
  }
}

void InterpolateSystem::SetGameObjects(
  const std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>> &objects)
{
  game_objects_ = objects;
}

void InterpolateSystem::SetPlayerId(const UUIDv4::UUID &player_id) { player_id_ = player_id; }

}// namespace chroma::app::states::network
