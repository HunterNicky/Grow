#include <iostream>
#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/client/render/Window.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/UIManager.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/PanelBuilder.h"
#include "chroma/client/ui/panels/PanelFactory.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"

namespace chroma::client::ui {

UIManager::UIManager() : command_queue_(std::make_unique<app::command::CommandQueue>())
{
  RegisterPanels();

  panel_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::PanelEvent>(
    [this](shared::event::ui::PanelEvent &event) { this->OnPanelEvent(event); });
}

void UIManager::OnPanelEvent(shared::event::ui::PanelEvent &panel_event)
{
  switch (panel_event.GetAction()) {
  case shared::event::ui::Action::Close: {
    auto action = [this, panel_id = panel_event.GetPanelId()]() { this->DoClosePanel(panel_id); };
    command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
    break;
  }
  case shared::event::ui::Action::Open: {
    auto action = [this, panel_id = panel_event.GetPanelId()]() { this->DoOpenPanel(panel_id); };
    command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
    break;
  }
  }
}

void UIManager::DoOpenPanel(const panel::PanelID panel_id)
{
  const render::RenderConfig config;
  const Vector2 screen_size = {static_cast<float>(config.virtual_width), static_cast<float>(config.virtual_height)};
  const Vector2 panel_size = { static_cast<float>(config.virtual_width), static_cast<float>(config.virtual_height) };

  auto panel = panel_factory_.Create(panel_id, screen_size, panel_size);
  if (panel) { panel_stack_.push_back(panel); }
}

void UIManager::DoClosePanel(const panel::PanelID panel_id)
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

Rectangle UIManager::GetCenteredRect(Vector2 parent_size, float width, float height)
{
  return { (parent_size.x - width) / 2.0F, (parent_size.y - height) / 2.0F, width, height };
}

void UIManager::RegisterPanels()
{
  panel_factory_.Register(panel::PanelID::MainMenuPanel, [this](Vector2 screen_size, Vector2 panel_size) {
    const Rectangle bounds = this->GetCenteredRect(screen_size, panel_size.x, panel_size.y);
    (void) panel_size;
    auto on_click_callback = [this](const std::string &button_id) {
      shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
      shared::event::EventBus::Dispatch(event);
    };

    return panel::PanelBuilder::Create(panel::PanelID::MainMenuPanel, bounds)
      .AddButton("Play_Singleplayer", "Play SinglePlayer", on_click_callback)
      .AddButton("Play_Multiplayer", "Play Multiplayer", on_click_callback)
      .AddButton("Options", "Options", on_click_callback)
      .AddButton("Exit", "Exit", on_click_callback)
      .CenterPanel()
      .Build();
  });

  panel_factory_.Register(panel::PanelID::MainBackgroundPanel, [this](Vector2 screen_size, Vector2 panel_size) {
    const Rectangle bounds = this->GetCenteredRect(screen_size, panel_size.x, panel_size.y);
    auto on_click_callback = [this](const std::string &button_id) {
      shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
      shared::event::EventBus::Dispatch(event);
    };

    return panel::PanelBuilder::Create(panel::PanelID::MainBackgroundPanel, bounds)
      .AddBackgroundTexture("assets/backgrounds/main.png")
      .Build();
  });

  panel_factory_.Register(panel::PanelID::OptionsMenuPanel, [this](Vector2 screen_size, Vector2 panel_size) {
    const Rectangle bounds = this->GetCenteredRect(screen_size, panel_size.x, panel_size.y);
    auto on_click_callback = [this](const std::string &button_id) {
      shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
      shared::event::EventBus::Dispatch(event);
    };

    return panel::PanelBuilder::Create(panel::PanelID::OptionsMenuPanel, bounds)
      .AddButton("Video", "Video", on_click_callback)
      .AddButton("Audio", "Audio", on_click_callback)
      .AddButton("Back", "Back", on_click_callback)
      .CenterPanel()
      .Build();
  });

  panel_factory_.Register(panel::PanelID::AudioOptionsPanel, [this](Vector2 screen_size, Vector2 panel_size) {
    const Rectangle bounds = this->GetCenteredRect(screen_size, panel_size.x, panel_size.y);
    auto on_click_callback = [this](const std::string &button_id) {
      shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
      shared::event::EventBus::Dispatch(event);
    };

    return panel::PanelBuilder::Create(panel::PanelID::AudioOptionsPanel, bounds)
      .AddSlider("GeneralVolume", "General", 0.0F, 100.0F, 0.0F)
      .AddSlider("MusicVolume", "Music", 0.0F, 100.0F, 0.0F)
      .AddSlider("SFXVolume", "SFX", 0.0F, 100.0F, 0.0F)
      .AddButton("AudioBack", "Back", on_click_callback)
      .CenterPanel()
      .Build();
  });

  panel_factory_.Register(panel::PanelID::VideoOptionsPanel, [this](Vector2 screen_size, Vector2 panel_size) {
    const Rectangle bounds = this->GetCenteredRect(screen_size, panel_size.x, panel_size.y);
    auto on_click_callback = [this](const std::string &button_id) {
      shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
      shared::event::EventBus::Dispatch(event);
    };

    return panel::PanelBuilder::Create(panel::PanelID::VideoOptionsPanel, bounds)
      .AddToggle("Bloom",
        "Bloom",
        true,
        [](bool is_on) {
          (void)is_on;
          std::cout << "bloom toggle\n";
        })
      .AddToggle("Scanlines",
        "Scanlines",
        true,
        [](bool is_on) {
          (void)is_on;
          std::cout << "Scanlines toggle\n";
        })
      .AddButton("VideoBack", "Back", on_click_callback)
      .CenterPanel()
      .Build();
  });
}

}// namespace chroma::client::ui