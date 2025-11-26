#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/states/menu/PauseState.h"
#include "chroma/app/states/menu/OptionsMenuState.h"

namespace chroma::app::layer::game {

GameLayer::GameLayer() : Layer("GameLayer") {
    state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::StateEvent>(
    [this](shared::event::state::StateEvent &event) { this->OnStateEvent(event); });
}

GameLayer::~GameLayer() = default;

void GameLayer::OnUpdate(const float delta_time)
{
  if (!IsActive()) { return; }

  command_queue_->Process();
  auto current_state = GetCurrentState();
  if (current_state) { current_state->OnUpdate(delta_time); }
}

void GameLayer::OnRender()
{
  if (!IsActive()) { return; }

  state_machine_->OnRenderAll();
}

void GameLayer::RegisterStates() {
    state_factory_.Register(
    states::StateID::PauseState, []() { return std::make_shared<states::menu::PauseState>(); });

    state_factory_.Register(
    states::StateID::OptionsMenuState, []() { return std::make_shared<states::menu::OptionsMenuState>(states::StateID::PauseState); });
}

void GameLayer::OnStateEvent(shared::event::state::StateEvent &state_event)
{
  switch (state_event.GetAction()) {
  case shared::event::state::Action::Pop: { 
    auto state = state_machine_->GetStateByName("GameState");

    if(state_event.GetStateId() == states::StateID::PauseState && command_queue_->IsEmpty())
    {
      if (const auto game_state = std::dynamic_pointer_cast<states::GameState>(state)) {
        game_state->SetPaused(false);
      }
    }

    auto action = [this]() { this->state_machine_->PopState(); };
    command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
    break;
  }
  case shared::event::state::Action::Push: {

    if (state_event.GetStateId() == states::StateID::PauseState || state_event.GetStateId() == states::StateID::OptionsMenuState) {
      auto game_state = std::dynamic_pointer_cast<states::GameState>(this->GetCurrentState());
      if (game_state) {
        game_state->SetPaused(true);
      }
    }

    auto action = [this, state_id = state_event.GetStateId()]() 
    {
      auto state = this->state_factory_.Create(state_id);
      if (state) { this->state_machine_->PushState(state); }
    };
    command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
    break;
  }
  }
}

}// namespace chroma::app::layer::game
