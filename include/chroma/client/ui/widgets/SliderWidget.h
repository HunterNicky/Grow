#pragma once

#include <raylib.h>
#include <string>
#include <utility>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::widget {

class SliderWidget : public Widget
{
public:
  SliderWidget(const std::string &id,
    Rectangle bounds,
    std::string label,
    int min_value,
    int max_value,
    int initial_value);

  ~SliderWidget() override = default;

  void OnUpdate(const float delta_time, const UIContext &context) override;
  void OnRender() override;

  int GetValue() const { return value_; }

private:
  int value_;
  int min_value_;
  int max_value_;
  bool is_dragging_ = false;

  std::string label_;

  const float bar_thickness_ = 4.0F;
  const float handle_radius_ = 10.0F;

  float GetHandleX() const;
  bool IsMouseOverHandle(Vector2 mouse_pos) const;
};

}// namespace chroma::client::ui::widget