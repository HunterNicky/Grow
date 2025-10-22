#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "chroma/shared/events/Event.h"
#include "chroma/app/states/StateMachine.h"
#include "chroma/app/states/State.h"

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

  [[nodiscard]] const std::string &GetName() const;
  [[nodiscard]] bool IsActive() const;
  void SetActive(bool active);

  void PushState(const std::shared_ptr<states::State> &state);
  void PopState();

  [[nodiscard]] std::shared_ptr<states::State> GetCurrentState();

protected:
  std::string name_;
  bool active_;

  std::unique_ptr<states::StateMachine> state_machine_;
};
}// namespace chroma::app::layer
