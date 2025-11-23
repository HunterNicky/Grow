#include "chroma/shared/factory/WeaponFactory.h"

#include "chroma/shared/core/components/Fist.h"
#include "chroma/shared/core/components/Javelin.h"
#include "chroma/shared/core/components/Spear.h"
#include "chroma/shared/core/components/Weapon.h"

#include <memory>

namespace chroma::shared::factory {
std::shared_ptr<core::component::Weapon> WeaponFactory::CreateWeaponByType(core::component::WeaponType type)
{
  switch (type) {
  case core::component::WeaponType::SPEAR:
    return std::make_shared<core::component::Spear>();
    // case core::component::WeaponType::SWORD:
    //     return std::make_shared<core::component::Sword>();

    // case core::component::WeaponType::BOW:
    //     return std::make_shared<core::component::Bow>();

    // case core::component::WeaponType::AXE:
    //     return std::make_shared<core::component::Axe>();

  case core::component::WeaponType::JAVELIN:
    return std::make_shared<core::component::Javelin>();
  case core::component::WeaponType::FIST:
    return std::make_shared<core::component::Fist>();
  default:
    return std::make_shared<core::component::Weapon>();
  }
}

}// namespace chroma::shared::factory