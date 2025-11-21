#include <memory>
#include <string>
#include <utility>

#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/game/MainMenuLayer.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/MainMenuState.h"
#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/StateEvent.h"

namespace chroma::app::layer::game {
MainMenuLayer::MainMenuLayer(std::string name) : Layer(std::move(name))
{
  auto main_menu_state = std::make_shared<chroma::app::states::menu::MainMenuState>();
  state_machine_->PushState(main_menu_state);

  state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::StateEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

MainMenuLayer::~MainMenuLayer() = default;

void MainMenuLayer::OnEvent(shared::event::Event &event)
{
  if (event.GetType() == shared::event::Event::Type::StateEvent) {
    auto state_event = dynamic_cast<shared::event::state::StateEvent &>(event);
    switch (state_event.GetAction()) {
    case shared::event::state::Action::Pop: {
      auto action = [this]() { this->state_machine_->PopState(); };
      command_queue_->Push(std::make_unique<command::FunctionalCommand>(action));
      break;
    }
    case shared::event::state::Action::Push: {
      auto action = [this, state_id = state_event.GetStateId()]() { this->DoPushState(state_id); };
      command_queue_->Push(std::make_unique<command::FunctionalCommand>(action));
      break;
    }
    }
  }
}

void MainMenuLayer::DoPushState(const states::StateID state_id)
{
  switch (state_id) {
  case states::StateID::MainMenuState: {
    auto state = std::make_shared<states::menu::MainMenuState>();
    state_machine_->PushState(state);
    break;
  }
  case states::StateID::OptionsMenuState: {
    auto state = std::make_shared<states::menu::OptionsMenuState>();
    state_machine_->PushState(state);
    break;
  }
  }
}

}// namespace chroma::app::layer::game