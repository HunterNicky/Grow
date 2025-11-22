#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/State.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"

namespace chroma::app::states::menu {
OptionsMenuState::OptionsMenuState() : State("OptionsMenu")
{
  shared::event::ui::PanelEvent panel_open_ev(
    shared::event::ui::Action::Open, client::ui::panel::PanelID::OptionsMenuPanel);
  shared::event::EventBus::Dispatch(panel_open_ev);

  button_click_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::ButtonClickEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

OptionsMenuState::~OptionsMenuState()
{
  shared::event::ui::PanelEvent panel_event(
    shared::event::ui::Action::Close, client::ui::panel::PanelID::OptionsMenuPanel);
  shared::event::EventBus::Dispatch(panel_event);
}

void OptionsMenuState::OnUpdate(const float delta_time) { (void)delta_time; }

void OptionsMenuState::OnRender() {}

void OptionsMenuState::OnEvent(shared::event::Event &event)
{
  auto btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent &>(event);
  if (btn_event.GetId() == "Video") {
    shared::event::ui::PanelEvent panel_close_event(shared::event::ui::Action::Close, client::ui::panel::PanelID::OptionsMenuPanel);
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelEvent panel_open_event(shared::event::ui::Action::Open, client::ui::panel::PanelID::VideoOptionsPanel);
    shared::event::EventBus::Dispatch(panel_open_event);
  } else if (btn_event.GetId() == "Audio") {
    shared::event::ui::PanelEvent panel_close_event(shared::event::ui::Action::Close, client::ui::panel::PanelID::OptionsMenuPanel);
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelEvent panel_open_event(shared::event::ui::Action::Open, client::ui::panel::PanelID::AudioOptionsPanel);
    shared::event::EventBus::Dispatch(panel_open_event);
  } else if (btn_event.GetId() == "Back") {
    shared::event::state::StateEvent pop_event(shared::event::state::Action::Pop);
    shared::event::EventBus::Dispatch(pop_event);
    shared::event::state::StateEvent push_event(shared::event::state::Action::Push, StateID::MainMenuState);
    shared::event::EventBus::Dispatch(push_event);
  } else if (btn_event.GetId() == "VideoBack") {
    shared::event::ui::PanelEvent panel_close_event(shared::event::ui::Action::Close, client::ui::panel::PanelID::VideoOptionsPanel);
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelEvent panel_open_event(shared::event::ui::Action::Open, client::ui::panel::PanelID::OptionsMenuPanel);
    shared::event::EventBus::Dispatch(panel_open_event);
  } else if (btn_event.GetId() == "AudioBack") {
    shared::event::ui::PanelEvent panel_close_event(shared::event::ui::Action::Close, client::ui::panel::PanelID::AudioOptionsPanel);
    shared::event::EventBus::Dispatch(panel_close_event);
    shared::event::ui::PanelEvent panel_open_event(shared::event::ui::Action::Open, client::ui::panel::PanelID::OptionsMenuPanel);
    shared::event::EventBus::Dispatch(panel_open_event);
  }
}
}// namespace chroma::app::states::menu