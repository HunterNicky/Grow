#include "chroma/shared/core/components/Speed.h"    
#include "chroma/shared/core/components/Component.h"

#include <raylib.h>

namespace chroma::shared::core::component {

Speed::Speed() : speed_{0.0F, 0.0F} {
    type_ = ComponentType::SPEED;
}

Speed::Speed(float speed) : speed_{speed, speed} {
    type_ = ComponentType::SPEED;
}

Speed::Speed(Vector2 speed) : speed_{speed} {
    type_ = ComponentType::SPEED;
}

Vector2 Speed::GetSpeed() const { return speed_; }

void Speed::SetSpeed(Vector2 new_speed) { speed_ = new_speed; }

} // namespace chroma::shared::core::component