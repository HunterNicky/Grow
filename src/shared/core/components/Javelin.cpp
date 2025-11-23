#include "chroma/shared/core/components/Javelin.h"

namespace chroma::shared::core::component {
Javelin::Javelin() : Weapon(WeaponType::JAVELIN, 5, 40.0F, 1.0F, 1.5F)
{
  type_ = ComponentType::WEAPON;
  SetSize(Vector2{ 20.0F, GetRange() });
  SetSpritePath("assets/sprites/player/weapons/randi-javelin.json");
}
}// namespace chroma::shared::core::component