#pragma once

#include <memory>
#include "chroma/shared/core/components/Weapon.h"

namespace chroma::shared::factory {

class WeaponFactory {
public:
    static std::shared_ptr<core::component::Weapon> CreateWeaponByType(core::component::WeaponType type);
};

} // namespace chroma::shared::factory