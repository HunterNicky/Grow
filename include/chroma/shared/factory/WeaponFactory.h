#pragma once

#include "chroma/shared/core/components/Weapon.h"
#include <memory>

namespace chroma::shared::factory {

class WeaponFactory
{
public:
  static std::shared_ptr<core::component::Weapon> Create(core::component::WeaponType type);
};

}// namespace chroma::shared::factory