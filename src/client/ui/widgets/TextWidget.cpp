#include "chroma/client/ui/widgets/TextWidget.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {

// NOLINTBEGIN
TextWidget::TextWidget(std::string id, const Rectangle bounds, std::string text, const int font_size, const Color color)
  : Widget(std::move(id), bounds), text_(std::move(text)), font_size_(font_size), color_(color)
{}
// NOLINTEND

void TextWidget::OnUpdate(const float delta_time, const UIContext &context)
{
  (void)delta_time;
  (void)context;
}

void TextWidget::OnRender()
{
  if (text_.empty()) { return; }

  const auto text_width = static_cast<float>(MeasureText(text_.c_str(), font_size_));

  const float text_x = bounds_.x + (bounds_.width / 2.0F) - (text_width / 2.0F);
  const float text_y = bounds_.y + (bounds_.height / 2.0F) - (static_cast<float>(font_size_) / 2.0F);

  DrawText(text_.c_str(), static_cast<int>(text_x), static_cast<int>(text_y), font_size_, color_);
}

void TextWidget::SetText(const std::string &text) { text_ = text; }

void TextWidget::SetFontSize(const int size) { font_size_ = size; }

void TextWidget::SetColor(const Color color) { color_ = color; }

}// namespace chroma::client::ui::widget