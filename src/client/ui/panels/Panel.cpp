#include <memory>
#include <numeric>
#include <raylib.h>
#include <utility>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/client/ui/widgets/BackGroundWidget.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::panel {
Panel::Panel(const PanelID panel_id, Rectangle bounds)
  : bounds_(bounds), id_(panel_id), is_active_(true), is_visible_(true), background_texture_()
{}

Panel::~Panel()
{
  if (background_texture_.id != 0) { UnloadTexture(background_texture_); }
}

void Panel::OnUpdate(const float delta_time, const UIContext &context)
{
  for (auto &widget : widgets_) { widget->OnUpdate(delta_time, context); }
}

void Panel::OnRender()
{
  DrawTexture(background_texture_, static_cast<int>(bounds_.x), static_cast<int>(bounds_.y), WHITE);
  for (auto &widget : widgets_) { widget->OnRender(); }
}

void Panel::CenterWidgets(float spacing)
{
    std::vector<widget::Widget*> content_widgets;
    for (const auto& w : widgets_) {
        if (dynamic_cast<widget::BackGroundWidget*>(w.get()) == nullptr) {
            content_widgets.push_back(w.get()); 
        }
    }

    if (content_widgets.empty()) { return; }

    float total_height = 0.0F;
    for (const auto& w : content_widgets) {
        total_height += w->GetBounds().height;
    }

    total_height += spacing * static_cast<float>(content_widgets.size() - 1);
    float start_y = bounds_.y + ((bounds_.height - total_height) / 2.0F);

    for (auto& w : content_widgets) {
        Rectangle b = w->GetBounds();

        b.x = bounds_.x + ((bounds_.width - b.width) / 2.0F);
        b.y = start_y;
        w->SetBounds(b);

        start_y += b.height + spacing;
    }

}

void Panel::AddWidget(std::unique_ptr<widget::Widget> widget) { widgets_.push_back(std::move(widget)); }

void Panel::SetIsActive(const bool is_active) { is_active_ = is_active; }

void Panel::SetIsVisible(const bool is_visible) { is_visible_ = is_visible; }

bool Panel::GetIsActive() const { return is_active_; }

bool Panel::GetIsVisible() const { return is_visible_; }

PanelID Panel::GetID() const { return id_; }

void Panel::SetBackgroundTexture(const Texture2D &texture)
{
  background_texture_ = texture;
  background_texture_.width = static_cast<int>(bounds_.width);
  background_texture_.height = static_cast<int>(bounds_.height);
}

}// namespace chroma::client::ui::panel