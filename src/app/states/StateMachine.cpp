#include <memory>

#include "chroma/app/events/Event.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateMachine.h"

namespace chroma::app::layer::states {
void StateMachine::OnUpdate(const float delta_time)
{
  if (!states_.empty() && states_.top()->IsActive()) { states_.top()->OnUpdate(delta_time); }
}

void StateMachine::OnFixedUpdate(const float fixed_delta_time)
{
  if (!states_.empty() && states_.top()->IsActive()) { states_.top()->OnFixedUpdate(fixed_delta_time); }
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
