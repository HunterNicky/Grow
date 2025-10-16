#pragma once 

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
class Speed : public Component {
public:
    Speed();
    Speed(float speed);
    ~Speed() override = default;
    float GetSpeed() const;
    void SetSpeed(float new_speed);
private:    
    float speed_;

};
} // namespace chroma::shared::core::component