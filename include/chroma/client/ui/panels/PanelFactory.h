#pragma once

#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"

#include <functional>
#include <memory>
#include <raylib.h>
#include <unordered_map>

namespace chroma::client::ui::panel {
class PanelFactory
{
public:
  void Register(const PanelID panel_id, std::function<std::shared_ptr<Panel>(Vector2, Vector2)> recipe);
  std::shared_ptr<panel::Panel> Create(const PanelID panel_id, Vector2 screen_size, Vector2 panel_size);

private:
  std::unordered_map<PanelID, std::function<std::shared_ptr<Panel>(Vector2, Vector2)>> factory_;
};
}// namespace chroma::client::ui::panel