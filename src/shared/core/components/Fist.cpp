#include "chroma/shared/core/components/Fist.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Weapon.h"

namespace chroma::shared::core::component {
Fist::Fist() : Weapon(WeaponType::FIST, 1, 5.0F, 0.5F, 0.8F, 0.0F, { 0.0F, 0.0F })
{
  type_ = ComponentType::WEAPON;
  SetSpritePath("assets/sprites/player/weapons/randi-fist.json");
}
}// namespace chroma::shared::core::component