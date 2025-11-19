#include "chroma/shared/core/components/Run.h"

namespace chroma::shared::core::component {

Run::Run(bool is_running, float speed_factor)
    : is_running_(is_running), factor_speed_(speed_factor)
{
    type_ = ComponentType::RUN;
}

Run::Run()
{
    type_ = ComponentType::RUN;
}

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