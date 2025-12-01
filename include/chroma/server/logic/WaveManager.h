#pragma once

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/world/WorldNavigation.h"

#include <cstdint>
#include <memory>
#include <raylib.h>

namespace chroma::server::logic {

class WaveManager
{
public:
  WaveManager();

  void Update(float delta_time);

  [[nodiscard]] uint32_t GetCurrentWave() const;

private:
  void StartNextWave();
  static void SpawnEnemies(uint32_t count);
  static std::shared_ptr<shared::core::GameObject> BuildEnemyAt(const Vector2 &spawn_pos);
  static Vector2 FindWalkableNear(const std::shared_ptr<shared::core::GameObject> &player_obj,
    const std::shared_ptr<shared::core::component::WorldNavigation> &world_nav);
  [[nodiscard]] static bool IsCurrentWaveCleared();

  uint32_t current_wave_;
  uint32_t max_waves_;
  float spawn_interval_;
  float time_since_last_check_;
};

}// namespace chroma::server::logic
