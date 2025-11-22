#pragma once

#include <raylib.h>
#include <string>
#include <utility>

#include "chroma/client/ui/UIContext.h"

namespace chroma::client::ui::widget {
class Widget
{
public:
  Widget(const std::string& id, Rectangle bounds) : id_(std::move(id)), bounds_(bounds) {};
  virtual ~Widget() = default;
  virtual void OnUpdate(const float delta_time, const UIContext &context) = 0;
  virtual void OnRender() = 0;

  void SetBounds(const Rectangle &bounds);
  [[nodiscard]] Rectangle GetBounds() const;

protected:
  std::string id_;
  Rectangle bounds_;
};
}// namespace chroma::client::ui::widget