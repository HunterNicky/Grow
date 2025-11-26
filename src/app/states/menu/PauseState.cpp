#include <iostream>
#include <raylib.h>

#include "chroma/app/layers/LayerIdentifiers.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/PauseState.h"
#include "chroma/client/render/Window.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/layer/LayerEvent.h"

namespace chroma::app::states::menu {
PauseState::PauseState() : State("PauseState")
{
  shared::event::ui::PanelEvent panel_event(
    shared::event::ui::Action::Open, client::ui::panel::PanelID::PausePanel);
  shared::event::EventBus::Dispatch(panel_event);

  button_click_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::ButtonClickEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

PauseState::~PauseState()
{
  shared::event::ui::PanelEvent panel_event(
    shared::event::ui::Action::Close, client::ui::panel::PanelID::PausePanel);
  shared::event::EventBus::Dispatch(panel_event);
}


void PauseState::OnUpdate(const float delta_time) { (void)delta_time; }

void PauseState::OnRender(){}

void PauseState::OnEvent(shared::event::Event &event)
{
  auto btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent &>(event);
  if (btn_event.GetId() == "Resume") {
    shared::event::state::StateEvent pop_state_event(shared::event::state::Action::Pop, StateID::PauseState);
    shared::event::EventBus::Dispatch(pop_state_event);
    
  } else if (btn_event.GetId() == "Options") {
  } else if (btn_event.GetId() == "MainMenu") {
    shared::event::layer::LayerEvent pop_game_layer_event(shared::event::layer::Action::Pop, layer::LayerID::GameLayer);
    shared::event::EventBus::Dispatch(pop_game_layer_event);

    shared::event::state::StateEvent pop_network_state_event(shared::event::state::Action::Pop, StateID::NetworkState);
    shared::event::EventBus::Dispatch(pop_network_state_event);

    shared::event::layer::LayerEvent push_main_menu_layer_event(
      shared::event::layer::Action::Push, layer::LayerID::MenuLayer);
    shared::event::EventBus::Dispatch(push_main_menu_layer_event);
  } 
}
}// namespace chroma::app::states::menu