#pragma once

#include "chroma/client/ui/widgets/Widget.h"

#include <raylib.h>
#include <functional>

namespace chroma::client::ui::widget {
class ToggleWidget : public Widget
{
public:
  ToggleWidget(const std::string &id, Rectangle bounds, std::string label, bool is_toggled);
  ~ToggleWidget() override = default;
  virtual void OnUpdate(const float delta_time, const UIContext &context) override;
  virtual void OnRender() override;
  void SetOnToggle(std::function<void(bool)> on_toggle);

private:
  bool is_toggled_;
  bool is_hovered_;
  std::string label_;
  std::function<void(bool)> on_toggle_;
};
}// namespace chroma::client::ui::widget