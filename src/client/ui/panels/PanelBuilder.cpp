#include "chroma/client/ui/panels/PanelBuilder.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/client/ui/widgets/Button.h"
#include "chroma/client/ui/widgets/SliderWidget.h"
#include "chroma/client/ui/widgets/ToggleWidget.h"

#include <functional>
#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

namespace chroma::client::ui::panel {

PanelBuilder::PanelBuilder(const PanelID panel_id, Rectangle bounds) : panel_(std::make_shared<panel::Panel>(panel_id, bounds))
{
  
  const float margin_top = 40;
  const float margin_side = 20;

  cursor_x_ = bounds.x + margin_side;
  cursor_y_ = bounds.y + margin_top;
}

PanelBuilder PanelBuilder::Create(const PanelID panel_id, Rectangle bounds) { return {panel_id, bounds}; }

PanelBuilder &PanelBuilder::AddButton(const std::string &id,
  const std::string &text,
  std::function<void(const std::string &)> on_click)
{
  const float width = 400;
  const float height = 100;

  const Rectangle rect = { cursor_x_, cursor_y_, width, height };

  auto btn = std::make_unique<widget::Button>(id, rect, text);
  btn->setOnClick(std::move(on_click));

  panel_->AddWidget(std::move(btn));

  cursor_y_ += height + item_padding_;

  return *this;
}

PanelBuilder &PanelBuilder::AddBackgroundTexture(const std::string &texture_path)
{
  const Texture2D texture = LoadTexture(texture_path.c_str());
  panel_->SetBackgroundTexture(texture);
  return *this;
}

PanelBuilder &PanelBuilder::CenterPanel()
{
  panel_->CenterWidgets(item_padding_);
  return *this;
}

PanelBuilder &PanelBuilder::AddSlider(const std::string &id, const std::string &label, int min, int max, int initial)
{
  const float slider_width = 360.0F;
  const float slider_height = 80.0F;

  const Rectangle rect = { cursor_x_ + item_padding_, cursor_y_, slider_width, slider_height };

  auto slider = std::make_unique<widget::SliderWidget>(id, rect, label, min, max, initial);
  panel_->AddWidget(std::move(slider));

  cursor_y_ += slider_height + item_padding_;

  return *this;
}

PanelBuilder &PanelBuilder::AddToggle(const std::string &id,
  const std::string &label,
  bool is_toggled,
  std::function<void(bool)> on_toggle)
{
  const float width = 120;
  const float height = 50;

  auto toggle = std::make_unique<client::ui::widget::ToggleWidget>(
    id, Rectangle{ cursor_x_, cursor_y_, width, height }, label, is_toggled);

  toggle->SetOnToggle(std::move(on_toggle));
  panel_->AddWidget(std::move(toggle));

  cursor_y_ += height + item_padding_;

  return *this;
}

PanelBuilder &PanelBuilder::AddSpace(float height)
{
  cursor_y_ += height;
  return *this;
}

PanelBuilder &PanelBuilder::SetPadding(float padding)
{
  item_padding_ = padding;
  return *this;
}

std::shared_ptr<panel::Panel> PanelBuilder::Build() { return std::move(panel_); }


}// namespace chroma::client::ui::panel