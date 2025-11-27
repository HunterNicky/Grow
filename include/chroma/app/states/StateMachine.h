#pragma once

#include <memory>
#include <stack>

#include "chroma/app/states/State.h"

namespace chroma::app::states {
class StateMachine
{
public:
  StateMachine() = default;
  ~StateMachine() = default;

  StateMachine(const StateMachine &) = delete;
  StateMachine(StateMachine &&) = delete;
  StateMachine &operator=(const StateMachine &) = delete;
  StateMachine &operator=(StateMachine &&) = delete;

  void OnUpdate(const float delta_time);
  void OnFixedUpdate(const float fixed_delta_time);
  void OnRender();

  void OnEvent(shared::event::Event &event);
  std::shared_ptr<State> GetStateByName(const std::string &name);

  void PushState(const std::shared_ptr<State> &state);
  void PopState();

  [[nodiscard]] std::shared_ptr<State> GetCurrentState();
  void OnRenderAll();

private:
  std::stack<std::shared_ptr<State>> states_;
};
}// namespace chroma::app::states
