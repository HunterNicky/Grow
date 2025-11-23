#pragma once

#include "chroma/shared/core/components/Weapon.h"

namespace chroma::shared::core::component {
class Fist : public Weapon
{
public:
  Fist();
  ~Fist() override = default;

  Fist(const Fist &) = delete;
  Fist &operator=(const Fist &) = delete;
  Fist(Fist &&) noexcept = default;
  Fist &operator=(Fist &&) noexcept = default;
};

}// namespace chroma::shared::core::component