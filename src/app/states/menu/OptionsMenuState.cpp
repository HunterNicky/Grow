#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/SaveSettingsEvent.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"

namespace chroma::app::states::menu {
OptionsMenuState::OptionsMenuState(const StateID state_to_return)
  : State("OptionsMenu"), state_to_return_(state_to_return)
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

  shared::event::SaveSettingsEvent save_event;
  shared::event::EventBus::Dispatch(save_event);
}

void OptionsMenuState::OnUpdate(const float delta_time) { (void)delta_time; }

void OptionsMenuState::OnRender() {}

void OptionsMenuState::OnEvent(shared::event::Event &event)
{
  auto *btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent *>(&event);
  if (!btn_event) { return; }

  const auto &id = btn_event->GetId();
  auto dispatch = [](auto &&e) { shared::event::EventBus::Dispatch(e); };

  using PanelID = client::ui::panel::PanelID;
  using UIAction = shared::event::ui::Action;

  auto switchPanel = [&](PanelID close, PanelID open) {
    dispatch(shared::event::ui::PanelEvent(UIAction::Close, close));
    dispatch(shared::event::ui::PanelEvent(UIAction::Open, open));
  };

  if (id == "Video") {
    switchPanel(PanelID::OptionsMenuPanel, PanelID::VideoOptionsPanel);
  } else if (id == "Audio") {
    switchPanel(PanelID::OptionsMenuPanel, PanelID::AudioOptionsPanel);
  } else if (id == "Back") {
    using StateAction = shared::event::state::Action;
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::OptionsMenuState));
    dispatch(shared::event::state::StateEvent(StateAction::Push, state_to_return_));
  } else if (id == "VideoBack") {
    switchPanel(PanelID::VideoOptionsPanel, PanelID::OptionsMenuPanel);
  } else if (id == "AudioBack") {
    switchPanel(PanelID::AudioOptionsPanel, PanelID::OptionsMenuPanel);
  }
}
}// namespace chroma::app::states::menu