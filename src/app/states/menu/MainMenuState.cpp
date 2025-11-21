#include <iostream>
#include <string>

#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/MainMenuState.h"
#include "chroma/client/ui/panels/MenuPanel.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/layer/LayerEvent.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelCloseEvent.h"
#include "chroma/shared/events/ui/PanelOpenEvent.h"
#include "chroma/app/layers/LayerIdentifiers.h"

namespace chroma::app::states::menu {
MainMenuState::MainMenuState() : State("Menu")
{
  shared::event::ui::PanelOpenEvent panel_open_ev("Menu");
  shared::event::EventBus::Dispatch(panel_open_ev);

  button_click_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::ButtonClickEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

MainMenuState::~MainMenuState()
{
  shared::event::ui::PanelCloseEvent panel_event("Menu");
  shared::event::EventBus::Dispatch(panel_event);
}

void MainMenuState::OnUpdate(const float delta_time) { (void)delta_time; }

void MainMenuState::OnRender() {}

void MainMenuState::OnEvent(shared::event::Event &event)
{
  auto btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent &>(event);
  if (btn_event.GetId() == "Play_Singleplayer") {
    shared::event::layer::LayerEvent push_layer_event(shared::event::layer::Action::Push, layer::LayerID::GameLayer);
    shared::event::EventBus::Dispatch(push_layer_event);
  } else if (btn_event.GetId() == "Play_Multiplayer") {
    shared::event::layer::LayerEvent push_layer_event(shared::event::layer::Action::Push, layer::LayerID::NetworkLayer);
    shared::event::EventBus::Dispatch(push_layer_event);
  } else if (btn_event.GetId() == "Options") {
    shared::event::state::StateEvent pop_event(shared::event::state::Action::Pop);
    shared::event::EventBus::Dispatch(pop_event);
    shared::event::state::StateEvent push_event(shared::event::state::Action::Push, StateID::OptionsMenuState);
    shared::event::EventBus::Dispatch(push_event);
  } else if (btn_event.GetId() == "Exit") {
    std::cout << "MainMenuState recebeu evento para Exit" << '\n';
  }
}
}// namespace chroma::app::layer::state::menu