#include "chroma/client/ui/UIManagerBus.h"
#include "chroma/client/ui/UIManager.h"

#include <memory>
#include <utility>

namespace chroma::client::ui {
std::unique_ptr<UIManager> UIManagerBus::ui_manager_ = nullptr;

void UIManagerBus::SetUIManager(std::unique_ptr<UIManager> &ui_manager)
{
  if (!ui_manager_) { ui_manager_ = std::move(ui_manager); }
}

UIManager *UIManagerBus::GetUIManager()
{
  if (ui_manager_) { return ui_manager_.get(); }
  return nullptr;
}
}// namespace chroma::client::ui