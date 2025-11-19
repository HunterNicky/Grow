#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/shared/events/ui/PanelCloseEvent.h"
#include "chroma/shared/events/ui/PanelOpenEvent.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::client::ui {
class UIManager
{
public:
  UIManager();
  ~UIManager() = default;

  void OnOpenPanel(shared::event::ui::PanelOpenEvent& event);
  void DoOpenPanel(const std::string &panel_id);
  void OnClosePanel(shared::event::ui::PanelCloseEvent& event);
  void DoClosePanel(const std::string &panel_id);

  void OnUpdate(const float delta_time);
  void OnRender() const;

private:
  UIContext context_;
  std::vector<std::shared_ptr<panel::Panel>> panel_stack_;
  std::unique_ptr<app::command::CommandQueue> command_queue_;

  shared::event::Subscription panel_open_sub_;
  shared::event::Subscription panel_close_sub_;
};
}// namespace chroma::client::ui