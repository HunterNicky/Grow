#pragma once

#include "chroma/shared/core/GameObject.h"
#include <memory>
#include <unordered_map>
#include <uuid_v4.h>

namespace chroma::app::states::network {

class InterpolateSystem
{
public:
  InterpolateSystem() = default;
  ~InterpolateSystem() = default;

  InterpolateSystem(const InterpolateSystem &) = default;
  InterpolateSystem &operator=(const InterpolateSystem &) = default;
  InterpolateSystem(InterpolateSystem &&) noexcept = default;
  InterpolateSystem &operator=(InterpolateSystem &&) noexcept = default;

  void Interpolate(
    const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &new_snapshot,
    uint64_t delta_time);
  void Update(uint64_t delta_time);

  void SetGameObjects(
    const std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>
      &objects);
  void SetPlayerId(const UUIDv4::UUID &player_id);

  [[nodiscard]] uint64_t GetTimeLastSnapshot() const;
  void SetSnapshotInterval(uint64_t interval);

private:
  std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>> game_objects_;
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> past_game_objects_;
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> target_game_objects_;
  UUIDv4::UUID player_id_;

  uint64_t interp_elapsed_ = 0;
  uint64_t snapshot_interval_ = 50;
  uint64_t time_last_snapshot_ = 0;

  void InterpolatePosition(const std::shared_ptr<shared::core::GameObject> &past_object,
    const std::shared_ptr<shared::core::GameObject> &target_object,
    std::shared_ptr<shared::core::GameObject> &out_object,
    float alpha) const;
};

}// namespace chroma::app::states::network