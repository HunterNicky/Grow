#include "chroma/shared/core/components/Run.h"

namespace chroma::shared::core::component {

Run::Run(bool is_running, float factor_speed)
    : is_running_(is_running), factor_speed_(factor_speed)
{
    type_ = ComponentType::RUN;
}

Run::Run() : is_running_(false), factor_speed_(1.5F)
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