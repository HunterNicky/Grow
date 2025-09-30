#pragma once

#include <memory>
#include <stack>

#include "chroma/app/layers/states/State.h"

namespace chroma::app::layer::state {
class StateMachine
{
public:
  StateMachine() = default;
  ~StateMachine() = default;

  StateMachine(const StateMachine &) = delete;
  StateMachine(StateMachine &&) = delete;
  StateMachine &operator=(const StateMachine &) = delete;
  StateMachine &operator=(StateMachine &&) = delete;

  void OnUpdate(const float deltaTime);
  void OnFixedUpdate(const float fixedDeltaTime);
  void OnRender();

  void OnEvent(event::Event &event);

  void PushState(const std::shared_ptr<State> &state);
  void PopState();

  [[nodiscard]] std::shared_ptr<State> GetCurrentState();

private:
  std::stack<std::shared_ptr<State>> states_;
};
}// namespace chroma::app::layer::state
