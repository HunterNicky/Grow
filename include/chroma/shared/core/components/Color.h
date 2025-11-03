#pragma once 

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
    class Color : public Component {
    public:
        explicit Color(float r, float g, float b, float a);
        Color() = default;
        Color(const Color &) = default;
        Color(Color &&) = delete;
        Color &operator=(const Color &) = default;
        Color &operator=(Color &&) = delete;
        ~Color() override = default;

        [[nodiscard]] float GetRed() const;
        [[nodiscard]] float GetGreen() const;
        [[nodiscard]] float GetBlue() const;
        [[nodiscard]] float GetAlpha() const;

        void SetRed(float r);
        void SetGreen(float g);
        void SetBlue(float b);
        void SetAlpha(float a);

        void SetColor(float r, float g, float b, float a);

    private:
        float red_ = 0.0F;
        float green_ = 0.0F;
        float blue_ = 0.0F;
        float alpha_ = 0.0F;
    };
}
