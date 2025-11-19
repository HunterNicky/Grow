#include "chroma/client/ui/widgets/SliderWidget.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <raylib.h>
#include <sstream>
#include <string>
#include <utility>

namespace chroma::client::ui::widget {

SliderWidget::SliderWidget(const std::string &id,
  Rectangle bounds,
  std::string label,
  float min_value,
  float max_value,
  float initial_value)
  : Widget(id, bounds), min_value_(min_value), max_value_(max_value), label_(std::move(label))
{
  value_ = std::clamp(initial_value, min_value_, max_value_);
}

float SliderWidget::GetHandleX() const
{
  const float normalized_value = (value_ - min_value_) / (max_value_ - min_value_);
  return bounds_.x + (normalized_value * bounds_.width);
}

bool SliderWidget::IsMouseOverHandle(Vector2 mouse_pos) const
{
  const Vector2 handle_center = { GetHandleX(), bounds_.y + (bounds_.height / 2.0F) };
  return CheckCollisionPointCircle(mouse_pos, handle_center, handle_radius_);
}

void SliderWidget::OnUpdate(const float delta_time, const UIContext &context)
{
  (void)delta_time;
  const Vector2 mouse_pos = context.GetMousePos();
  const bool is_left_mouse_pressed = context.GetIsLeftMousePressed();
  const bool is_left_mouse_down = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

  if (is_left_mouse_pressed && IsMouseOverHandle(mouse_pos)) {
    is_dragging_ = true;
  } else if (!is_left_mouse_down) {
    is_dragging_ = false;
  }

  if (is_dragging_ && is_left_mouse_down) {
    float new_x = mouse_pos.x;
    new_x = std::clamp(new_x, bounds_.x, bounds_.x + bounds_.width);

    const float normalized_value = (new_x - bounds_.x) / bounds_.width;
    const float new_value = min_value_ + (normalized_value * (max_value_ - min_value_));

    value_ = std::clamp(new_value, min_value_, max_value_);
  }
}

void SliderWidget::OnRender()
{
  const float center_y = bounds_.y + (bounds_.height / 2.0F);
  const float handle_x = GetHandleX();

  DrawText(label_.c_str(), (int)bounds_.x, (int)bounds_.y - 25, 20, DARKGRAY);

  DrawLineEx({ bounds_.x, center_y }, { bounds_.x + bounds_.width, center_y }, bar_thickness_, LIGHTGRAY);

  DrawLineEx({ bounds_.x, center_y }, { handle_x, center_y }, bar_thickness_, BLUE);

  DrawCircleV({ handle_x, center_y }, handle_radius_, DARKBLUE);
  DrawCircleLines(static_cast<int>(handle_x), static_cast<int>(center_y), handle_radius_, BLACK);

  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2) << value_;
  const std::string value_text = oss.str();

  DrawText(value_text.c_str(), (int)(bounds_.x + bounds_.width + 15.0F), (int)(center_y - 10.0F), 20, BLACK);
}

}// namespace chroma::client::ui::widget