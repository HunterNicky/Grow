#include <iostream>
#include <string>

#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/client/ui/panels/OptionsMenuPanel.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelCloseEvent.h"
#include "chroma/shared/events/ui/PanelOpenEvent.h"

namespace chroma::app::states::menu {
OptionsMenuState::OptionsMenuState() : State("OptionsMenu")
{
  shared::event::ui::PanelOpenEvent panel_open_ev("Options");
  shared::event::EventBus::Dispatch(panel_open_ev);

  button_click_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::ButtonClickEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

OptionsMenuState::~OptionsMenuState()
{
  shared::event::ui::PanelCloseEvent panel_event("Options");
  shared::event::EventBus::Dispatch(panel_event);
}

void OptionsMenuState::OnUpdate(const float delta_time) { (void)delta_time; }

void OptionsMenuState::OnRender() {}

void OptionsMenuState::OnEvent(shared::event::Event &event)
{
  auto btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent &>(event);
  if (btn_event.GetId() == "Video") {
    shared::event::ui::PanelCloseEvent panel_close_event("Options");
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelOpenEvent panel_open_event("VideoOptions");
    shared::event::EventBus::Dispatch(panel_open_event);
  } else if (btn_event.GetId() == "Audio") {
    std::cout << "OptionsMenuState recebeu evento para Audio" << '\n';
    shared::event::ui::PanelCloseEvent panel_close_event("Options");
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelOpenEvent panel_open_event("AudioOptions");
    shared::event::EventBus::Dispatch(panel_open_event);
  } else if (btn_event.GetId() == "Back") {
    shared::event::state::StateEvent pop_event(shared::event::state::Action::Pop);
    shared::event::EventBus::Dispatch(pop_event);
    shared::event::state::StateEvent push_event(shared::event::state::Action::Push, StateID::MainMenuState);
    shared::event::EventBus::Dispatch(push_event);
  } else if (btn_event.GetId() == "VideoBack") {
    shared::event::ui::PanelCloseEvent panel_close_event("VideoOptions");
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelOpenEvent panel_open_event("Options");
    shared::event::EventBus::Dispatch(panel_open_event);
  } else if (btn_event.GetId() == "AudioBack") {
    shared::event::ui::PanelCloseEvent panel_close_event("AudioOptions");
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelOpenEvent panel_open_event("Options");
    shared::event::EventBus::Dispatch(panel_open_event);
  }
}
}// namespace chroma::app::states::menu