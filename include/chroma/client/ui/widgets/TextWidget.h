#pragma once

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"
#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {

class TextWidget : public Widget
{
public:
    TextWidget(std::string id,
               Rectangle bounds, 
               std::string text,
               int font_size, 
               Color color);

    void OnUpdate(const float delta_time, const UIContext &context) override;
    void OnRender() override;

    void SetText(const std::string& text);
    void SetFontSize(int size);
    void SetColor(Color color);

protected:
    std::string text_;
    int font_size_;
    Color color_;
};

} // namespace chroma::client::ui::widget