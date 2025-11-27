#include "chroma/shared/core/components/Bow.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Weapon.h"

#include <raylib.h>

namespace chroma::shared::core::component {
Bow::Bow() : Weapon(WeaponType::BOW, 10, 40.0F, 1.0F, 2.5F)
{
  type_ = ComponentType::WEAPON;
  SetSize(Vector2{ 20.0F, GetRange() });
  SetSpritePath("assets/sprites/player/weapons/bow-projectile.json");
}
}// namespace chroma::shared::core::component