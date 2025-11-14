#pragma once 

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>

namespace chroma::shared::core::component {

class Health : public Component {
public:
    explicit Health(float max_health);
    explicit Health();
    ~Health() override = default;

    // default copy/move semantics (trivial for float members)
    Health(const Health&) = default;
    Health& operator=(const Health&) = default;
    Health(Health&&) noexcept = default;
    Health& operator=(Health&&) noexcept = default;

    void TakeDamage(float amount);
    void Heal(float amount);
    
    [[nodiscard]] float GetCurrentHealth() const;
    [[nodiscard]] float GetMaxHealth() const;

    void SetMaxHealth(float max_health);
    void SetCurrentHealth(float current_health);

    void DrawHealth(Vector2 position, Vector2 size) const;

private:
    float current_health_;
    float max_health_;
};

} // namespace chroma::shared::core::component