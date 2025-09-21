#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "chroma/app/events/Event.h"

namespace chroma::app::layer {
class StateMachine
{
  [[maybe_unused]] bool TODO = true;
};
class State
{
  [[maybe_unused]] bool TODO = true;
};

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
  virtual void OnUpdate([[maybe_unused]] const float deltaTime);
  virtual void OnRender();
  virtual void OnEvent([[maybe_unused]] event::Event &event);

  [[nodiscard]] const std::string &GetName() const;
  [[nodiscard]] bool IsActive() const;
  void SetActive(bool active);

  void static PushState(const std::unique_ptr<State> &state);
  static void PopState();

  [[nodiscard]] State static *GetCurrentState();

protected:
  std::string name_;
  bool active_;
  std::unique_ptr<StateMachine> state_machine_;
};
}// namespace chroma::app::layer
