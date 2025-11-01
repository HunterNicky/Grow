#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Component.h"
#include <raylib.h>

namespace chroma::shared::core::component {
Movement::Movement() : direction_{ 0.0F, 0.0F } { type_ = ComponentType::MOVEMENT; }

Movement::Movement(Vector2 direction) : direction_(direction) { type_ = ComponentType::MOVEMENT; }

Vector2 Movement::GetDirection() const { return direction_; }

void Movement::SetDirection(Vector2 new_direction) { direction_ = new_direction; }

void Movement::Reset() { direction_ = Vector2{ 0.0F, 0.0F }; }
}// namespace chroma::shared::core::component