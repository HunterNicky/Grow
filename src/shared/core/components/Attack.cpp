#include "chroma/shared/core/components/Attack.h"

namespace chroma::shared::core::component {
Attack::Attack() : is_attacking_(false) { type_ = ComponentType::ATTACK; }
Attack::Attack(bool is_attacking) : is_attacking_(is_attacking) { type_ = ComponentType::ATTACK; }
bool Attack::IsAttacking() const { return is_attacking_; }

void Attack::SetAttacking(bool attacking) { is_attacking_ = attacking; }

}// namespace chroma::shared::core::component