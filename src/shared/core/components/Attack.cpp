#include "chroma/shared/core/components/Attack.h"

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
Attack::Attack() : { type_ = ComponentType::ATTACK; }
Attack::Attack(bool is_attacking) : is_attacking_(is_attacking) { type_ = ComponentType::ATTACK; }
bool Attack::IsAttacking() const { return is_attacking_; }

void Attack::SetAttacking(bool attacking) { is_attacking_ = attacking; }

}// namespace chroma::shared::core::component