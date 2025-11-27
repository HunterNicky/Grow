#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateFactory.h"
#include "chroma/app/states/StateMachine.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/state/StateEvent.h"

namespace chroma::app::layer {
class Layer
{
public:
  Layer(const Layer &) = delete;
  Layer(Layer &&) = delete;
  Layer &operator=(const Layer &) = delete;
  Layer &operator=(Layer &&) = delete;
  explicit Layer(std::string name);
  virtual ~Layer() = default;

  virtual void OnAttach();
  virtual void OnDetach();

  virtual void OnUpdate(const float delta_time);
  virtual void OnFixedUpdate(const float fixed_delta_time);
  virtual void OnRender();

  virtual void OnEvent(shared::event::Event &event);

  virtual void OnStateEvent(shared::event::state::StateEvent &state_event) = 0;

  [[nodiscard]] const std::string &GetName() const;
  [[nodiscard]] bool IsActive() const;
  void SetActive(bool active);

  void PushState(const std::shared_ptr<states::State> &state);
  void PopState();

  [[nodiscard]] std::shared_ptr<states::State> GetCurrentState();

  virtual void RegisterStates() = 0;

protected:
  std::string name_;
  bool active_;

  std::unique_ptr<states::StateMachine> state_machine_;
  states::StateFactory state_factory_;
  std::unique_ptr<app::command::CommandQueue> command_queue_;
  shared::event::Subscription state_sub_;
};
}// namespace chroma::app::layer
