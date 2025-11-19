#include <memory>
#include <raylib.h>
#include <string>
#include <utility>
#include <vector>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/UIManager.h"
#include "chroma/client/ui/panels/MenuPanel.h"
#include "chroma/client/ui/panels/OptionsMenuPanel.h"
#include "chroma/client/ui/panels/VideoOptionsPanel.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ui/PanelCloseEvent.h"
#include "chroma/shared/events/ui/PanelOpenEvent.h"
#include "chroma/client/ui/panels/AudioOptionsPanel.h"

namespace chroma::client::ui {

UIManager::UIManager() : command_queue_(std::make_unique<app::command::CommandQueue>())
{
  panel_open_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::PanelOpenEvent>(
    [this](shared::event::ui::PanelOpenEvent &event) { this->OnOpenPanel(event); });

  panel_close_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::PanelCloseEvent>(
    [this](shared::event::ui::PanelCloseEvent &event) { this->OnClosePanel(event); });
}

void UIManager::OnOpenPanel(shared::event::ui::PanelOpenEvent &event)
{
  auto action = [this, panel_id = std::move(event.GetId())]() { this->DoOpenPanel(panel_id); };

  command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
}

void UIManager::DoOpenPanel(const std::string &panel_id)
{
  const float panel_width = 300.0F;
  const float panel_height = 400.0F;
  const float screen_width = 1280.0F;
  const float screen_height = 720.0F;

  const Rectangle panel_bounds = {
    (screen_width - panel_width) / 2.0F, (screen_height - panel_height) / 2.0F, panel_width, panel_height
  };

  if (panel_id == "Menu") {
    auto menu_panel = std::make_shared<panel::MenuPanel>(panel_bounds);
    panel_stack_.push_back(menu_panel);
  } else if (panel_id == "Options") {
    auto options_panel = std::make_shared<panel::OptionsMenuPanel>(panel_bounds);
    panel_stack_.push_back(options_panel);
  } else if (panel_id == "VideoOptions") {
    auto video_options_panel = std::make_shared<panel::VideoOptionsPanel>(panel_bounds);
    panel_stack_.push_back(video_options_panel);
  } else if (panel_id == "AudioOptions") {
    auto audio_options_panel = std::make_shared<panel::AudioOptionsPanel>(panel_bounds);
    panel_stack_.push_back(audio_options_panel);
  } 
}

void UIManager::OnClosePanel(shared::event::ui::PanelCloseEvent &event)
{
  auto action = [this, panel_id = std::move(event.GetId())]() { this->DoClosePanel(panel_id); };

  command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
}

void UIManager::DoClosePanel(const std::string &panel_id)
{
  std::erase_if(panel_stack_, [panel_id](auto &panel) { return panel->GetID() == panel_id; });
}

void UIManager::OnUpdate(const float delta_time)
{
  context_.SetMousePos(GetMousePosition());
  context_.SetIsLeftMousePressed(IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
  command_queue_->Process();

  for (const std::shared_ptr<panel::Panel> &panel : panel_stack_) {
    if (panel->GetIsActive()) { panel->OnUpdate(delta_time, context_); }
  }
}

void UIManager::OnRender() const
{
  for (const std::shared_ptr<panel::Panel> &panel : panel_stack_) {
    if (panel->GetIsVisible()) { panel->OnRender(); }
  }
}
}// namespace chroma::client::ui