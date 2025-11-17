#include "chroma/shared/core/components/Weapon.h"

namespace chroma::shared::core::component {
Weapon::Weapon(WeaponType type, int32_t damage, float range)
    : type_(type), damage_(damage), range_(range){}

WeaponType Weapon::GetType() const {
    return type_;
}

void Weapon::SetType(WeaponType type) {
    type_ = type;
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

} // namespace chroma::shared::core::component