#pragma once

#include <functional>
#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelFactory.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/ui/PanelEvent.h"

namespace chroma::client::ui {
class UIManager
{
public:
  UIManager();
  ~UIManager() = default;

  void OnUpdate(const float delta_time);
  void OnRender() const;

  void OnPanelEvent(shared::event::ui::PanelEvent &panel_event);
  void DoOpenPanel(const panel::PanelID panel_id);
  void DoClosePanel(const panel::PanelID panel_id);

private:
  static Rectangle GetCenteredRect(Vector2 parent_size, float width, float height);
  void RegisterPanels();

private:
  UIContext context_;
  panel::PanelFactory panel_factory_;
  std::vector<std::shared_ptr<panel::Panel>> panel_stack_;
  std::unique_ptr<app::command::CommandQueue> command_queue_;

  shared::event::Subscription panel_sub_;
};
}// namespace chroma::client::ui