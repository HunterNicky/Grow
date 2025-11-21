#include "chroma/shared/core/components/Whip.h"

namespace chroma::shared::core::component {
Whip::Whip()
: Weapon(WeaponType::WHIP, 5, 40.0F, 1.0F, 0.5F)
{
    type_ = ComponentType::WEAPON;
    SetSize(Vector2{20.0F, GetRange()});
    SetSpritePath("assets/sprites/player/weapons/randi-whip.json");
}
} // namespace chroma::shared::core::component