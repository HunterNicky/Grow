#include "chroma/app/states/StateMachine.h"
#include "chroma/app/states/State.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <stack>
#include <string>

namespace chroma::app::states {
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

void StateMachine::OnRenderAll()
{
  std::stack<std::shared_ptr<State>> temp_stack;

  while (!states_.empty()) {
    auto current_state = states_.top();
    states_.pop();
    temp_stack.push(current_state);
  }

  while (!temp_stack.empty()) {
    auto state = temp_stack.top();
    temp_stack.pop();
    state->OnRender();
    states_.push(state);
  }
}

void StateMachine::OnEvent(shared::event::Event &event)
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

std::shared_ptr<State> StateMachine::GetStateByName(const std::string &name)
{
  std::stack<std::shared_ptr<State>> temp_stack;
  std::shared_ptr<State> found_state = nullptr;

  while (!states_.empty()) {
    auto current_state = states_.top();
    states_.pop();
    temp_stack.push(current_state);

    if (current_state->GetName() == name) {
      found_state = current_state;
      break;
    }
  }

  while (!temp_stack.empty()) {
    states_.push(temp_stack.top());
    temp_stack.pop();
  }

  return found_state;
}


}// namespace chroma::app::states
