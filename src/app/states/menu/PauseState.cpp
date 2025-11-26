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
#include "chroma/shared/events/layer/LayerEvent.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"

namespace chroma::app::states::menu {
PauseState::PauseState() : State("PauseState")
{
  shared::event::ui::PanelEvent panel_event(shared::event::ui::Action::Open, client::ui::panel::PanelID::PausePanel);
  shared::event::EventBus::Dispatch(panel_event);

  button_click_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::ButtonClickEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

PauseState::~PauseState()
{
  shared::event::ui::PanelEvent panel_event(shared::event::ui::Action::Close, client::ui::panel::PanelID::PausePanel);
  shared::event::EventBus::Dispatch(panel_event);
}


void PauseState::OnUpdate(const float delta_time) { (void)delta_time; }

void PauseState::OnRender() {}

void PauseState::OnEvent(shared::event::Event &event)
{
  auto *btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent *>(&event);
  if (!btn_event) { return; }

  const auto &id = btn_event->GetId();
  auto dispatch = [](auto &&e) { shared::event::EventBus::Dispatch(e); };

  using StateAction = shared::event::state::Action;
  using LayerAction = shared::event::layer::Action;
  using LayerID = layer::LayerID;
  using UIAction = shared::event::ui::Action;
  using PanelID = client::ui::panel::PanelID;

  if (id == "Resume") {
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::PauseState));
    dispatch(shared::event::ui::PanelEvent(UIAction::Close, PanelID::PauseBackgroundPanel));
  } else if (id == "Options") {
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::PauseState));
    dispatch(shared::event::state::StateEvent(StateAction::Push, StateID::OptionsMenuState));
  } else if (id == "MainMenu") {
    dispatch(shared::event::layer::LayerEvent(LayerAction::Pop, LayerID::GameLayer));
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::NetworkState));
    dispatch(shared::event::layer::LayerEvent(LayerAction::Push, LayerID::MenuLayer));
    dispatch(shared::event::ui::PanelEvent(UIAction::Close, PanelID::PauseBackgroundPanel));
  }
}
}// namespace chroma::app::states::menu