#include <memory>
#include <string>
#include <utility>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/game/MainMenuLayer.h"
#include "chroma/app/states/StateFactory.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/MainMenuState.h"
#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"

namespace chroma::app::layer::game {
MainMenuLayer::MainMenuLayer(std::string name) : Layer(std::move(name))
{
  state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::StateEvent>(
    [this](shared::event::state::StateEvent &event) { this->OnStateEvent(event); });

  shared::event::ui::PanelEvent bg_main(
    shared::event::ui::Action::Open, client::ui::panel::PanelID::MainBackgroundPanel);
  shared::event::EventBus::Dispatch(bg_main);

  auto main_menu_state = std::make_shared<chroma::app::states::menu::MainMenuState>();
  state_machine_->PushState(main_menu_state);
}

MainMenuLayer::~MainMenuLayer()
{
  shared::event::ui::PanelEvent bg_main(
    shared::event::ui::Action::Close, client::ui::panel::PanelID::MainBackgroundPanel);
  shared::event::EventBus::Dispatch(bg_main);
};

void MainMenuLayer::RegisterStates()
{
  state_factory_.Register(
    states::StateID::MainMenuState, []() { return std::make_shared<states::menu::MainMenuState>(); });

  state_factory_.Register(states::StateID::OptionsMenuState,
    []() { return std::make_shared<states::menu::OptionsMenuState>(states::StateID::MainMenuState); });
}

void MainMenuLayer::OnStateEvent(shared::event::state::StateEvent &state_event)
{
  switch (state_event.GetAction()) {
  case shared::event::state::Action::Pop: {
    auto action = [this]() { this->state_machine_->PopState(); };
    command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
    break;
  }
  case shared::event::state::Action::Push: {
    auto action = [this, state_id = state_event.GetStateId()]() {
      auto state = this->state_factory_.Create(state_id);
      if (state) { this->state_machine_->PushState(state); }
    };
    command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
    break;
  }
  }
}

}// namespace chroma::app::layer::game