#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Component.h"
#include <algorithm>
#include <raylib.h>

namespace chroma::shared::core::component {

Health::Health() : current_health_(std::make_shared<float>(100.0F)), max_health_(std::make_shared<float>(100.0F))
{
    type_ = ComponentType::HEALTH;
}

Health::Health(float max_health): current_health_(std::make_shared<float>(max_health)), max_health_(std::make_shared<float>(max_health))
{
    type_ = ComponentType::HEALTH;
}

void Health::TakeDamage(float amount)
{
    *current_health_ -= amount;
    *current_health_ = std::max(*current_health_, 0.0F);
}

void Health::Heal(float amount)
{
    *current_health_ += amount;
    *current_health_ = std::min(*current_health_, *max_health_);
}

void Health::SetMaxHealth(float max_health)
{
    *max_health_ = max_health;
    *current_health_ = std::min(*current_health_, *max_health_);
}

void Health::SetCurrentHealth(float current_health)
{
    *current_health_ = std::clamp(current_health, 0.0F, *max_health_);
}

std::shared_ptr<float> Health::GetCurrentHealth() const
{
    return current_health_;
}

std::shared_ptr<float> Health::GetMaxHealth() const
{
    return max_health_;
}

void Health::DrawHealth(Vector2 position, Vector2 size) const
{
    float health_ratio = *current_health_ / *max_health_;
    health_ratio = std::clamp(health_ratio, 0.0F, 1.0F);

    DrawRectangleV(position, size, DARKGRAY);
    DrawRectangleV(position, { size.x * health_ratio, size.y }, GREEN);
    DrawRectangleLinesEx({ position.x, position.y, size.x, size.y }, 1.0F, BLACK);
}

} // namespace chroma::shared::core::component