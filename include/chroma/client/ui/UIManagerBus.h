#pragma once

#include "chroma/client/ui/UIManager.h"

#include <memory>

namespace chroma::client::ui {
class UIManagerBus
{
public:
  UIManagerBus() = delete;

  static void SetUIManager(std::unique_ptr<UIManager> &ui_manager);
  [[nodiscard]] static UIManager* GetUIManager();

private:
  static std::unique_ptr<UIManager> ui_manager_;

};
}// namespace chroma::client::ui