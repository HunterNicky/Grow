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
        Weapon(WeaponType type, int32_t damage, float range);

        [[nodiscard]]WeaponType GetType() const;
        void SetType(WeaponType type);

        [[nodiscard]]int32_t GetDamage() const;
        void SetDamage(int32_t damage);

        [[nodiscard]]float GetRange() const;
        void SetRange(float range);

        [[nodiscard]]bool IsEquipped() const;
        void SetEquipped(bool equipped);

        [[nodiscard]]bool IsAttacking() const;
        void SetAttacking(bool attacking);

    private:
        WeaponType type_;
        int32_t damage_;
        float range_;
        bool is_equipped_;
        bool is_attacking_;
    };
} // namespace chroma::shared::core::components