#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Component.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <raylib.h>

namespace chroma::shared::core::component {

Health::Health() : current_health_(std::make_shared<float>(100.0F)), max_health_(std::make_shared<float>(100.0F))
{
  type_ = ComponentType::HEALTH;
}

Health::Health(float max_health)
  : current_health_(std::make_shared<float>(max_health)), max_health_(std::make_shared<float>(max_health))
{
  type_ = ComponentType::HEALTH;
}

void Health::TakeDamage(const float amount)
{
  if (current_health_ == nullptr) { return; }
  
  const auto now = std::chrono::steady_clock::now();
  const auto ms_since_last = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_damage_time_).count();
  if (ms_since_last < static_cast<long long>(damage_cooldown_ms_)) { return; }
  
  *current_health_ -= amount;
  *current_health_ = std::max(*current_health_, 0.0F);
  last_damage_time_ = now;
}

void Health::Heal(const float amount) const
{
  if (current_health_ == nullptr) { return; }
  if (max_health_ == nullptr) { return; }
  *current_health_ += amount;
  *current_health_ = std::min(*current_health_, *max_health_);
}

void Health::SetMaxHealth(const float max_health) const
{
  if (current_health_ == nullptr) { return; }
  if (max_health_ == nullptr) { return; }
  *max_health_ = max_health;
  *current_health_ = std::min(*current_health_, *max_health_);
}

void Health::SetCurrentHealth(const float current_health) const
{
  if (current_health_ == nullptr) { return; }
  if (max_health_ == nullptr) { return; }
  *current_health_ = std::clamp(current_health, 0.0F, *max_health_);
}

std::shared_ptr<float> Health::GetCurrentHealth() const { return current_health_; }

std::shared_ptr<float> Health::GetMaxHealth() const { return max_health_; }

void Health::DrawHealth(const Vector2 position, const Vector2 size) const
{
  if (!current_health_ || !max_health_) { return; }
  const float cur = (*current_health_);
  const float max = (*max_health_);
  if (!std::isfinite(cur) || !std::isfinite(max) || max <= 0.0F) { return; }
  float health_ratio = cur / max;
  health_ratio = std::clamp(health_ratio, 0.0F, 1.0F);

  DrawRectangleV(position, size, DARKGRAY);
  DrawRectangleV(position, { size.x * health_ratio, size.y }, GREEN);
  DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 1.0F, BLACK);
}

void Health::SetDamageCooldown(const float cooldown_ms)
{
  damage_cooldown_ms_ = cooldown_ms;
}

}// namespace chroma::shared::core::component