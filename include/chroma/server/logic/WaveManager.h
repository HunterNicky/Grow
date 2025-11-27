#pragma once

#include <cstdint>

namespace chroma::server::logic {

class WaveManager
{
public:
  WaveManager();

  void Update(float delta_time);

  [[nodiscard]] uint32_t GetCurrentWave() const;

private:
  void StartNextWave();
  void SpawnEnemies(uint32_t count) const;
  [[nodiscard]] bool IsCurrentWaveCleared() const;

  uint32_t current_wave_;
  uint32_t max_waves_;
  float spawn_interval_;
  float time_since_last_check_;
};

}// namespace chroma::server::logic
