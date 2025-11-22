#include "chroma/app/states/network/InterpolateSystem.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/Health.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <raymath.h>
#include <unordered_map>
#include <uuid_v4.h>

namespace chroma::app::states::network {


void InterpolateSystem::OnPacketReceived(
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &new_snapshot,
  const uint64_t delta_time)
{
  if (past_game_objects_.empty()) {
    past_game_objects_ = new_snapshot;
    target_game_objects_ = new_snapshot;
    interp_elapsed_ = 0;
    time_last_snapshot_ = delta_time;
    return;
  }

  if (!new_snapshot.empty()) {
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
  const std::shared_ptr<shared::core::GameObject> &out_object,
  const float alpha) const
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

void InterpolateSystem::Update(const std::shared_ptr<shared::core::GameObjectManager> &manager,
  const uint64_t delta_time)
{
  if (past_game_objects_.empty() || target_game_objects_.empty()) { return; }

  interp_elapsed_ += delta_time;
  float alpha = static_cast<float>(interp_elapsed_) / static_cast<float>(snapshot_interval_);
  alpha = std::clamp(alpha, 0.0F, 1.0F);

  for (auto &[id, target_obj] : target_game_objects_) {
    if (!target_obj) { continue; }

    if (id == player_id_) { continue; }

    auto past_it = past_game_objects_.find(id);
    std::shared_ptr<shared::core::GameObject> past_obj = nullptr;
    if (past_it != past_game_objects_.end()) { past_obj = past_it->second; }

    if (!past_obj) { past_obj = target_obj; }

    if (manager->Exists(id)) {
      auto live_obj = manager->Get(id);
      if (live_obj) { InterpolatePosition(past_obj, target_obj, live_obj, alpha); }
    } else {
      const auto clone = target_obj->Clone();
      if (clone) {
        clone->SetNetRole(target_obj->GetNetRole());
        InterpolatePosition(past_obj, target_obj, clone, alpha);
        manager->Register(clone);
      }
    }
  }
}

void InterpolateSystem::SetPlayerId(const UUIDv4::UUID &player_id) { player_id_ = player_id; }

}// namespace chroma::app::states::network
