#include "chroma/shared/core/components/Spear.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Weapon.h"

namespace chroma::shared::core::component {
Spear::Spear() : Weapon(WeaponType::SPEAR, 15, 20.0F, 3.0F, 1.2F, 0.0F, { 0.0F, 0.0F })
{
  SetSpritePath("assets/sprites/player/weapons/randi-spear.json");
}

}// namespace chroma::shared::core::component