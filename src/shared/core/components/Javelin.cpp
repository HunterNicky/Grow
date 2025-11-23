#include "chroma/shared/core/components/Javelin.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Weapon.h"

#include <raylib.h>

namespace chroma::shared::core::component {
Javelin::Javelin() : Weapon(WeaponType::JAVELIN, 5, 40.0F, 1.0F, 1.5F)
{
  type_ = ComponentType::WEAPON;
  SetSize(Vector2{ 20.0F, GetRange() });
  SetSpritePath("assets/sprites/player/weapons/randi-javelin.json");
}
}// namespace chroma::shared::core::component