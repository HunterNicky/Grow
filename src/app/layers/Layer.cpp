#include <memory>
#include <string>
#include <utility>

#include "chroma/app/events/Event.h"
#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer {
Layer::Layer(std::string name) : name_(std::move(name)), active_(true), state_machine_(std::make_unique<StateMachine>())
{}

void Layer::OnAttach() {}

void Layer::OnDetach() {}

void Layer::OnUpdate([[maybe_unused]] const float deltaTime) {}

void Layer::OnFixedUpdate([[maybe_unused]] const float fixedDeltaTime) {}

void Layer::OnRender() {}

void Layer::OnEvent([[maybe_unused]] event::Event &event) {}

const std::string &Layer::GetName() const { return name_; }

bool Layer::IsActive() const { return active_; }

void Layer::SetActive(bool active) { active_ = active; }

// TODO: remover maybe_unused quando implementar
void Layer::PushState([[maybe_unused]] const std::unique_ptr<State> &state)
{
  // state_machine_->PushState(std::move(state));
}

void Layer::PopState()
{
  // state_machine_->PopState();
}

State *Layer::GetCurrentState()
{
  // return state_machine_->GetCurrentState();
  return nullptr;
}

}// namespace chroma::app::layer
