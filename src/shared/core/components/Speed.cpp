#include "chroma/shared/core/components/Speed.h"    
#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {

Speed::Speed() : speed_(0.0F) {
    type_ = ComponentType::SPEED;
}

Speed::Speed(float speed) : speed_(speed) {
    type_ = ComponentType::SPEED;
}

float Speed::GetSpeed() const { return speed_; }

void Speed::SetSpeed(float new_speed) { speed_ = new_speed; }

} // namespace chroma::shared::core::component