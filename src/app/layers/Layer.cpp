#include <memory>
#include <string>
#include <utility>

#include "chroma/app/events/Event.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/states/State.h"
#include "chroma/app/layers/states/StateMachine.h"

namespace chroma::app::layer {
Layer::Layer(std::string name)
  : name_(std::move(name)), active_(true), state_machine_(std::make_unique<state::StateMachine>())
{}

void Layer::OnAttach() {}

void Layer::OnDetach() {}

void Layer::OnUpdate(const float delta_time) { state_machine_->OnUpdate(delta_time); }

void Layer::OnFixedUpdate(const float fixed_delta_time) { state_machine_->OnFixedUpdate(fixed_delta_time); }

void Layer::OnRender() { state_machine_->OnRender(); }

void Layer::OnEvent(event::Event &event) { state_machine_->OnEvent(event); }

const std::string &Layer::GetName() const { return name_; }

bool Layer::IsActive() const { return active_; }

void Layer::SetActive(bool active) { active_ = active; }

void Layer::PushState(const std::shared_ptr<state::State> &state) { state_machine_->PushState(state); }

void Layer::PopState() { state_machine_->PopState(); }

std::shared_ptr<state::State> Layer::GetCurrentState() { return state_machine_->GetCurrentState(); }
}// namespace chroma::app::layer
