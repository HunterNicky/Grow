#pragma once

#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"

#include <functional>
#include <memory>
#include <raylib.h>
#include <string>

namespace chroma::client::ui::panel {

class PanelBuilder
{
public:
  static PanelBuilder Create(const PanelID panel_id, Rectangle bounds);

  PanelBuilder &
    AddButton(const std::string &id, const std::string &text, std::function<void(const std::string &)> on_click);

  PanelBuilder &AddSlider(const std::string &id, const std::string &label, int min, int max, int initial);
  PanelBuilder &AddToggle(const std::string &id, const std::string &label, bool is_toggled, std::function<void(bool)> on_toggle);

  PanelBuilder &AddSpace(float height);
  PanelBuilder &SetPadding(float padding);

  [[nodiscard]] std::shared_ptr<Panel> Build();

private:
  PanelBuilder(const PanelID panel_id, Rectangle bounds);

  std::shared_ptr<Panel> panel_;

  float cursor_x_;
  float cursor_y_;
  float item_padding_{ 20.0f };
};

}// namespace chroma::client::ui