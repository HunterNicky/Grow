#include "chroma/shared/core/components/Weapon.h"
#include <string>

namespace chroma::shared::core::component {

Weapon::Weapon(WeaponType type, int32_t damage, float range, float weight, float cooldown)
    : weapon_type_(type), damage_(damage), range_(range), weight_(weight), cooldown_(cooldown) 
    {
        type_ = ComponentType::WEAPON;
    }

WeaponType Weapon::GetWeaponType() const {
    return weapon_type_;
}

void Weapon::SetWeaponType(WeaponType type) {
    weapon_type_ = type;
}

int32_t Weapon::GetDamage() const {
    return damage_;
}

void Weapon::SetDamage(int32_t damage) {
    damage_ = damage;
}

float Weapon::GetRange() const {
    return range_;
}

void Weapon::SetRange(float range) {
    range_ = range;
}

float Weapon::GetWeight() const {
    return weight_;
}

void Weapon::SetWeight(float weight) {
    weight_ = weight;
}

float Weapon::GetCooldown() const {
    return cooldown_;
}

void Weapon::SetCooldown(float cooldown) {
    cooldown_ = cooldown;
}

std::string Weapon::WeaponTypeToPrefix(WeaponType type)
{
    switch(type) {
        case WeaponType::FIST:  return "fist";
        case WeaponType::SWORD: return "sword";
        case WeaponType::BOW:   return "bow";
        case WeaponType::AXE:   return "axe";
        case WeaponType::SPEAR: return "spear";
        default: return "unknown";
    }
}

} // namespace chroma::shared::core::component