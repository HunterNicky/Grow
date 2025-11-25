#include <memory>
#include <string>
#include <utility>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateMachine.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/state/StateEvent.h"

namespace chroma::app::layer {
Layer::Layer(std::string name)
  : name_(std::move(name)), active_(true), state_machine_(std::make_unique<states::StateMachine>()),
    command_queue_(std::make_unique<command::CommandQueue>())
{
  state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::StateEvent>(
    [this](shared::event::state::StateEvent &event) { this->OnStateEvent(event); });
}

void Layer::OnAttach() { RegisterStates(); }

void Layer::OnDetach() {}

void Layer::OnUpdate(const float delta_time)
{
  command_queue_->Process();
  state_machine_->OnUpdate(delta_time);
}

void Layer::OnFixedUpdate(const float fixed_delta_time) { state_machine_->OnFixedUpdate(fixed_delta_time); }

void Layer::OnRender() { state_machine_->OnRender(); }

void Layer::OnEvent(shared::event::Event &event) { state_machine_->OnEvent(event); }

void Layer::OnStateEvent(shared::event::state::StateEvent &state_event)
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

const std::string &Layer::GetName() const { return name_; }

bool Layer::IsActive() const { return active_; }

void Layer::SetActive(bool active) { active_ = active; }

void Layer::PushState(const std::shared_ptr<states::State> &state) { state_machine_->PushState(state); }

void Layer::PopState() { state_machine_->PopState(); }

std::shared_ptr<states::State> Layer::GetCurrentState() { return state_machine_->GetCurrentState(); }
}// namespace chroma::app::layer
