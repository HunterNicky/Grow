#include <functional>
#include <raylib.h>
#include <string>
#include <utility>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Button.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::widget {
Button::Button(const std::string &id, Rectangle bounds, std::string text)
  : Widget(id, bounds), text_(std::move(text)), is_hovered_(false)
{}

void Button::OnUpdate(const float delta_time, const UIContext &context)
{
  (void)delta_time;
  is_hovered_ = CheckCollisionPointRec(context.GetMousePos(), bounds_);

  if (is_hovered_ && context.GetIsLeftMousePressed()) {
    if (on_click_callback_) { on_click_callback_(id_); }
  }
}

void Button::OnRender()
{
  const float border_thickness = 4.0F;
  Color highlight_color = { .r = 210, .g = 210, .b = 210, .a = 255 };
  const Color shadow_color = { 80, 80, 80, 255 };
  const Color inner_border_color = BLACK;
  const int font_size = 28;

  if (is_hovered_) { highlight_color = { .r = 210, .g = 210, .b = 210, .a = 100 }; }

  DrawLineEx({ bounds_.x, bounds_.y }, { bounds_.x + bounds_.width, bounds_.y }, border_thickness, highlight_color);
  DrawLineEx({ bounds_.x, bounds_.y }, { bounds_.x, bounds_.y + bounds_.height }, border_thickness, highlight_color);

  DrawLineEx({ bounds_.x, bounds_.y + bounds_.height },
    { bounds_.x + bounds_.width, bounds_.y + bounds_.height },
    border_thickness,
    shadow_color);
  DrawLineEx({ bounds_.x + bounds_.width, bounds_.y },
    { bounds_.x + bounds_.width, bounds_.y + bounds_.height },
    border_thickness,
    shadow_color);

  DrawRectangleLinesEx({ bounds_.x + border_thickness,
                         bounds_.y + border_thickness,
                         bounds_.width - (border_thickness * 2),
                         bounds_.height - (border_thickness * 2) },
    border_thickness / 2,
    inner_border_color);


  const auto text_width = static_cast<float>(MeasureText(text_.c_str(), font_size));

  const int text_x = static_cast<int>(bounds_.x + ((bounds_.width - text_width) / 2.0F));
  const int text_y = static_cast<int>(bounds_.y + ((bounds_.height - font_size) / 2.0F));

  DrawText(text_.c_str(), text_x + 1, text_y + 1, font_size, BLACK);
  DrawText(text_.c_str(), text_x, text_y, font_size, WHITE);
}

void Button::setOnClick(std::function<void(const std::string &)> callback) { on_click_callback_ = std::move(callback); }
}// namespace chroma::client::ui::widget