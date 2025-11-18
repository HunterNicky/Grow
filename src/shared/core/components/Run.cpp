#include "chroma/shared/core/components/Run.h"

namespace chroma::shared::core::component {

bool Run::IsRunning() const {
    return is_running_;
}

void Run::SetRunning(bool running) {
    is_running_ = running;
}

float Run::GetSpeedFactor() const {
    if(is_running_)
    {
        return factor_speed_;
    }
    else
    {
        return 1.0F;
    }
}

void Run::SetSpeedFactor(float factor) {
    factor_speed_ = factor;
}

} // chroma::shared::core::component