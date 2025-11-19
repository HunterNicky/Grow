#pragma once 

#include <cstdint>

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
    enum class WeaponType : uint8_t {
        FIST = 0,
        SWORD = 1,
        BOW = 2,
        AXE = 3,
        SPEAR = 4,
    };

    class Weapon : public Component {
    public:
        Weapon(WeaponType type, int32_t damage, float range, float weight = 1.0F, float cooldown = 1.0F);

        [[nodiscard]]WeaponType GetWeaponType() const;
        void SetWeaponType(WeaponType type);

        [[nodiscard]]int32_t GetDamage() const;
        void SetDamage(int32_t damage);

        [[nodiscard]]float GetRange() const;
        void SetRange(float range);

        [[nodiscard]]float GetWeight() const;
        void SetWeight(float weight);

        [[nodiscard]]float GetCooldown() const;
        void SetCooldown(float cooldown);

    private:
        WeaponType weapon_type_;
        int32_t damage_;
        float range_;
        float weight_;
        float cooldown_;
    };
} // namespace chroma::shared::core::component