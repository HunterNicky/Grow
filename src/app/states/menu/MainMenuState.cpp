#include <iostream>
#include <string>

#include "chroma/app/states/State.h"
#include "chroma/app/states/menu/MainMenuState.h"
#include "chroma/client/ui/panels/MenuPanel.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/PopStateEvent.h"
#include "chroma/shared/events/state/PushStateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelCloseEvent.h"
#include "chroma/shared/events/ui/PanelOpenEvent.h"
#include "chroma/shared/events/layer/PushLayerEvent.h"

namespace chroma::app::layer::state::menu {
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
    std::cout << "MainMenuState recebeu evento para Play_Singleplayer" << '\n';
    shared::event::layer::PushLayerEvent push_layer_event("SinglePlayerGameLayer");
    shared::event::EventBus::Dispatch(push_layer_event);
  } else if (btn_event.GetId() == "Play_Multiplayer") {
    std::cout << "MainMenuState recebeu evento para Play_Multiplayer" << '\n';
    shared::event::layer::PushLayerEvent push_layer_event("MultiPlayerGameLayer");
    shared::event::EventBus::Dispatch(push_layer_event);
  } else if (btn_event.GetId() == "Options") {
    shared::event::state::PopStateEvent pop_event;
    shared::event::EventBus::Dispatch(pop_event);
    shared::event::state::PushStateEvent push_event("Options");
    shared::event::EventBus::Dispatch(push_event);
  } else if (btn_event.GetId() == "Exit") {
    std::cout << "MainMenuState recebeu evento para Exit" << '\n';
  }
}
}// namespace chroma::app::layer::state::menu