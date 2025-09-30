#include <memory>

#include "chroma/app/events/Event.h"
#include "chroma/app/layers/states/State.h"
#include "chroma/app/layers/states/StateMachine.h"

namespace chroma::app::layer::state {
void StateMachine::OnUpdate(const float deltaTime)
{
  if (!states_.empty() && states_.top()->IsActive()) { states_.top()->OnUpdate(deltaTime); }
}

void StateMachine::OnFixedUpdate(const float fixedDeltaTime)
{
  if (!states_.empty() && states_.top()->IsActive()) { states_.top()->OnFixedUpdate(fixedDeltaTime); }
}

void StateMachine::OnRender()
{
  if (!states_.empty() && states_.top()->IsActive()) { states_.top()->OnRender(); }
}

void StateMachine::OnEvent(event::Event &event)
{
  if (!states_.empty() && states_.top()->IsActive()) { states_.top()->OnEvent(event); }
}

void StateMachine::PushState(const std::shared_ptr<State> &state)
{
  if (state) {
    if (!states_.empty()) { states_.top()->SetActive(false); }
    states_.emplace(state);
    states_.top()->SetActive(true);
    states_.top()->OnAttach();
  }
}

void StateMachine::PopState()
{
  if (!states_.empty()) {
    states_.top()->OnDetach();
    states_.pop();
    if (!states_.empty()) { states_.top()->SetActive(true); }
  }
}

std::shared_ptr<State> StateMachine::GetCurrentState()
{
  if (!states_.empty()) { return states_.top(); }
  return nullptr;
}
}// namespace chroma::app::layer::state
