#pragma once

#include <raylib.h>
#include <string>
#include <functional>
#include <utility>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::widget {
class Button : public Widget
{
public:
  Button(const std::string& id, Rectangle bounds, std::string text);
  ~Button() override = default;
  virtual void OnUpdate(const float delta_time, const UIContext &context) override;
  virtual void OnRender() override;
  void setOnClick(std::function<void(const std::string&)> callback);

private:
  std::string text_;
  bool is_hovered_;

  std::function<void(const std::string&)> on_click_callback_;
};
}// namespace chroma::client::ui::widget