#include "chroma/shared/core/components/Weapon.h"
#include "chroma/shared/core/components/Component.h"
#include <cstdint>
#include <raylib.h>
#include <string>

namespace chroma::shared::core::component {

Weapon::Weapon()
  : position_{ 0.0F, 0.0F }, size_{ 16.0F, 16.0F }, weapon_type_(WeaponType::FIST), damage_(0), range_(0.0F),
    weight_(0.0F), cooldown_(0.0F), last_attack_time_(0.0F)
{
  type_ = ComponentType::WEAPON;
}

Weapon::Weapon(WeaponType type,
  int32_t damage,
  float range,
  float weight,
  float cooldown,
  float last_attack_time,
  Vector2 position)
  : position_(position), size_{ 16.0F, 16.0F }, weapon_type_(type), damage_(damage), range_(range), weight_(weight),
    cooldown_(cooldown), last_attack_time_(last_attack_time)
{
  type_ = ComponentType::WEAPON;
}

WeaponType Weapon::GetWeaponType() const { return weapon_type_; }

void Weapon::SetWeaponType(WeaponType type) { weapon_type_ = type; }

int32_t Weapon::GetDamage() const { return damage_; }

void Weapon::SetDamage(int32_t damage) { damage_ = damage; }

float Weapon::GetRange() const { return range_; }

void Weapon::SetRange(float range) { range_ = range; }

float Weapon::GetWeight() const { return weight_; }

void Weapon::SetWeight(float weight) { weight_ = weight; }

float Weapon::GetCooldown() const { return cooldown_; }

void Weapon::SetCooldown(float cooldown) { cooldown_ = cooldown; }

std::string Weapon::WeaponTypeToPrefix(WeaponType type)
{
  switch (type) {
  case WeaponType::FIST:
    return "fist";
  case WeaponType::SWORD:
    return "sword";
  case WeaponType::BOW:
    return "bow";
  case WeaponType::AXE:
    return "axe";
  case WeaponType::SPEAR:
    return "spear";
  case WeaponType::JAVELIN:
    return "javelin";
  default:
    return "unknown";
  }
}

const std::string &Weapon::GetSpritePath() const { return sprite_path_; }

void Weapon::SetSpritePath(const std::string &sprite_path) { sprite_path_ = sprite_path; }

float Weapon::GetLastAttackTime() const { return last_attack_time_; }

void Weapon::SetLastAttackTime(float time) { last_attack_time_ = time; }

Vector2 Weapon::GetPosition() const { return position_; }

void Weapon::SetPosition(const Vector2 &position) { position_ = position; }

Vector2 Weapon::GetSize() const { return size_; }

void Weapon::SetSize(const Vector2 &size) { size_ = size; }

void Weapon::Render()
{
  //    DrawRectangle(static_cast<int>(position_.x), static_cast<int>(position_.y), static_cast<int>(size_.x),
  //    static_cast<int>(size_.y), RED);
}

}// namespace chroma::shared::core::component