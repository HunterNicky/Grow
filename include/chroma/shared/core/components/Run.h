#pragma once

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {

class Run : public Component {
public:
    Run();
    Run(bool is_running, float speed_factor);
    ~Run() override = default;

    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;
    Run(Run&&) = delete;
    Run& operator=(Run&&) = delete;

    [[nodiscard]] bool IsRunning() const;
    void SetRunning(bool running);

    [[nodiscard]] float GetSpeedFactor() const;
    void SetSpeedFactor(float factor);

private:
    bool is_running_{ false };
    float factor_speed_{ 1.5F };  
};
} // chroma::shared::core::component