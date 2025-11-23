#include "chroma/client/ui/widgets/ToggleWidget.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

#include <functional>
#include <raylib.h>
#include <string>
#include <utility>

namespace chroma::client::ui::widget {
ToggleWidget::ToggleWidget(const std::string &id, Rectangle bounds, std::string label, bool is_toggled)
  : Widget(id, bounds), is_toggled_(is_toggled), is_hovered_(false), label_(std::move(label))
{}

void ToggleWidget::OnUpdate(const float delta_time, const UIContext &context)
{
  (void)delta_time;

  is_hovered_ = CheckCollisionPointRec(context.GetMousePos(), bounds_);

  if (is_hovered_ && context.GetIsLeftMousePressed()) {
    is_toggled_ = !is_toggled_;
    if (on_toggle_) { on_toggle_(is_toggled_); }
  }
}

void ToggleWidget::OnRender()
{
  const Rectangle track_bounds = bounds_;

  const float thumb_y_center = bounds_.y + (bounds_.height / 2.0F);
  const float thumb_size = bounds_.height * 0.85F;

  const float thumb_x_off = bounds_.x + (bounds_.height / 2.0F);
  const float thumb_x_on = bounds_.x + bounds_.width - (bounds_.height / 2.0F);

  Color track_color;
  auto thumb_color = WHITE;
  float thumb_x_center = 0;

  if (is_toggled_) {
    track_color = DARKBLUE;
    thumb_x_center = thumb_x_on;
  } else {
    track_color = GRAY;
    thumb_x_center = thumb_x_off;
  }

  if (is_hovered_) { thumb_color = LIGHTGRAY; }

  DrawRectangleRec(track_bounds, track_color);

  const float thumb_rect_x = thumb_x_center - (thumb_size / 2.0F);
  const float thumb_rect_y = thumb_y_center - (thumb_size / 2.0F);

  const Rectangle thumb_rect = { thumb_rect_x, thumb_rect_y, thumb_size, thumb_size };

  DrawRectangleRec(thumb_rect, thumb_color);

  const float label_x = bounds_.x + bounds_.width + 10.0F;
  const float label_y = bounds_.y + (bounds_.height / 2.0F) - 10.0F;
  const int font_size = 26;

  DrawText(label_.c_str(), static_cast<int>(label_x), static_cast<int>(label_y), font_size, WHITE);
}

void ToggleWidget::SetOnToggle(std::function<void(bool)> on_toggle) { on_toggle_ = std::move(on_toggle); }
}// namespace chroma::client::ui::widget