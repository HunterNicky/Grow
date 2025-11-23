#pragma once
#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {

class Attack : public Component
{
public:
  Attack();
  explicit Attack(bool is_attacking);

  bool IsAttacking() const;
  void SetAttacking(bool attacking);

private:
  bool is_attacking_{ false };
};

}// namespace chroma::shared::core::component
