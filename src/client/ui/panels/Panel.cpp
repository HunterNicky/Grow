#include <memory>
#include <raylib.h>
#include <utility>
#include <string>
#include <string_view>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::panel {
Panel::Panel(std::string id, Rectangle bounds) : bounds_(bounds), id_(std::move(id)), is_active_(true), is_visible_(true) {}

void Panel::OnUpdate(const float delta_time, const UIContext &context)
{
  for (auto &widget : widgets_) { widget->OnUpdate(delta_time, context); }
}

void Panel::OnRender()
{
  DrawRectangleRec(bounds_, { 0, 0, 0, 0 });
  for (auto &widget : widgets_) { widget->OnRender(); }
}

void Panel::AddWidget(std::unique_ptr<widget::Widget> widget)
{
  widgets_.push_back(std::move(widget));
}

void Panel::SetIsActive(const bool is_active)
{
  is_active_ = is_active;
}

void Panel::SetIsVisible(const bool is_visible)
{
  is_visible_ = is_visible;
}

bool Panel::GetIsActive() const
{
  return is_active_;
}

bool Panel::GetIsVisible() const
{
  return is_visible_;
}

std::string_view Panel::GetID() const
{
  return id_;
}
}// namespace chroma::client::ui::panel