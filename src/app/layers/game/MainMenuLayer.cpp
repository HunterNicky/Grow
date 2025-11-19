#include <memory>
#include <string>
#include <utility>

#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/game/MainMenuLayer.h"
#include "chroma/app/states/menu/MainMenuState.h"
#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/PopStateEvent.h"
#include "chroma/shared/events/state/PushStateEvent.h"

namespace chroma::app::layer::game {
MainMenuLayer::MainMenuLayer(std::string name) : Layer(std::move(name))
{
  auto main_menu_state = std::make_shared<chroma::app::layer::state::menu::MainMenuState>();
  state_machine_->PushState(main_menu_state);

  pop_state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::PopStateEvent>(
    [this](const shared::event::state::PopStateEvent &event) { this->OnPopState(event); });

  push_state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::PushStateEvent>(
    [this](shared::event::state::PushStateEvent &event) { this->OnPushState(event); });
}

MainMenuLayer::~MainMenuLayer() = default;

void MainMenuLayer::OnPopState(const shared::event::state::PopStateEvent &event)
{
  (void)event;
  auto action = [this]() { this->DoPopState(); };
  command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
}

void MainMenuLayer::OnPushState(shared::event::state::PushStateEvent &event)
{
  auto action = [this, state_id = std::move(event.GetStateId())]() { this->DoPushState(state_id); };
  command_queue_->Push(std::make_unique<app::command::FunctionalCommand>(action));
}

void MainMenuLayer::DoPopState() { state_machine_->PopState(); }

void MainMenuLayer::DoPushState(const std::string &state_id)
{
  if (state_id == "Menu") {
    auto state = std::make_shared<state::menu::MainMenuState>();
    state_machine_->PushState(state);
  } else if (state_id == "Options") {
    auto state = std::make_shared<state::menu::OptionsMenuState>();
    state_machine_->PushState(state);
  }
}

}// namespace chroma::app::layer::game