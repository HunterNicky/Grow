#include "chroma/client/ui/widgets/TextWidget.h"
#include <raylib.h>
#include <iostream>

namespace chroma::client::ui::widget {

TextWidget::TextWidget(const std::string& id, 
                       Rectangle bounds, 
                       const std::string& text, 
                       int font_size, 
                       Color color)
    : Widget(std::move(id), bounds),
      text_(text),
      font_size_(font_size),
      color_(color)
{
    // Apenas inicialização.
}

void TextWidget::OnUpdate(const float delta_time, const UIContext &context)
{
    (void)delta_time;
    (void)context;
}

void TextWidget::OnRender()
{
    if (text_.empty()) {
        return;
    }

    auto text_width = static_cast<float>(MeasureText(text_.c_str(), font_size_));

    const float text_x = bounds_.x + (bounds_.width / 2.0F) - (text_width / 2.0F);
    const float text_y = bounds_.y + (bounds_.height / 2.0F) - (static_cast<float>(font_size_) / 2.0F);

    DrawText(text_.c_str(), 
             static_cast<int>(text_x), 
             static_cast<int>(text_y), 
             font_size_, 
             color_);
}

void TextWidget::SetText(const std::string& text)
{
    text_ = text;
}

void TextWidget::SetFontSize(int size)
{
    font_size_ = size;
}

void TextWidget::SetColor(Color color)
{
    color_ = color;
}

} // namespace chroma::client::ui::widget