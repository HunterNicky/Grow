#pragma once

#include "chroma/shared/core/components/Component.h"

#include <chrono>
#include <memory>
#include <raylib.h>

namespace chroma::shared::core::component {

class Health : public Component
{
public:
  explicit Health(float max_health);
  explicit Health();
  ~Health() override = default;

  // default copy/move semantics (trivial for float members)
  Health(const Health &) = default;
  Health &operator=(const Health &) = default;
  Health(Health &&) noexcept = default;
  Health &operator=(Health &&) noexcept = default;

  void TakeDamage(float amount);
  void Heal(float amount) const;

  [[nodiscard]] std::shared_ptr<float> GetCurrentHealth() const;
  [[nodiscard]] std::shared_ptr<float> GetMaxHealth() const;

  void SetMaxHealth(float max_health) const;
  void SetCurrentHealth(float current_health) const;
  void SetDamageCooldown(float cooldown_ms);

  void DrawHealth(Vector2 position, Vector2 size) const;

private:
  std::shared_ptr<float> current_health_;
  std::shared_ptr<float> max_health_;
  
  std::chrono::steady_clock::time_point last_damage_time_{};
  float damage_cooldown_ms_{ 1000.0F };
};

}// namespace chroma::shared::core::component