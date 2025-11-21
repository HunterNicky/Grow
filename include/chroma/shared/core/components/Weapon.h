#pragma once 

#include <cstdint>
#include <raylib.h>

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
    enum class WeaponType : uint8_t {
        FIST = 0,
        SWORD = 1,
        BOW = 2,
        AXE = 3,
        SPEAR = 4,
        WHIP = 5
    };

    class Weapon : public Component {
    public:
        Weapon( WeaponType type, 
                int32_t damage,
                float range,
                float weight = 1.0F,
                float cooldown = 1.0F,
                float last_attack_time = 0.0F,
                Vector2 position = {0.0F, 0.0F}
        );

        Weapon();

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

        [[nodiscard]]const std::string& GetSpritePath() const;
        void SetSpritePath(const std::string& sprite_path);

        [[nodiscard]]float GetLastAttackTime() const;
        void SetLastAttackTime(float time);

        [[nodiscard]]Vector2 GetPosition() const;
        void SetPosition(const Vector2& position);

        [[nodiscard]]Vector2 GetSize() const;
        void SetSize(const Vector2& size);
        
        void Render() override;

        static std::string WeaponTypeToPrefix(WeaponType type);

    private:
        Vector2 position_;
        Vector2 size_;
        WeaponType weapon_type_;
        std::string sprite_path_;
        int32_t damage_;
        float range_;
        float weight_;
        float cooldown_;
        float last_attack_time_;
    };
} // namespace chroma::shared::core::component