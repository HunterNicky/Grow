#include "chroma/client/ui/panels/PanelFactory.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"

#include <functional>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include <utility>

namespace chroma::client::ui::panel {
void PanelFactory::Register(const PanelID panel_id, std::function<std::shared_ptr<Panel>(Vector2, Vector2)> recipe)
{
  factory_[panel_id] = std::move(recipe);
}

std::shared_ptr<panel::Panel> PanelFactory::Create(const PanelID panel_id, Vector2 screen_size, Vector2 panel_size)
{
  auto it = factory_.find(panel_id);
  if (it != factory_.end()) { return it->second(screen_size, panel_size); }
  return nullptr;
}
}// namespace chroma::client::ui::panel