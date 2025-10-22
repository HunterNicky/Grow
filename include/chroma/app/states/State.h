#pragma once

#include <string>

#include "chroma/shared/events/Event.h"

namespace chroma::app::layer::states {
class State
{
public:
  State(const State &) = default;
  State(State &&) = delete;
  State &operator=(const State &) = default;
  State &operator=(State &&) = delete;
  explicit State(std::string name);
  virtual ~State() = default;

  virtual void OnAttach();
  virtual void OnDetach();

  virtual void OnUpdate(const float delta_time);
  virtual void OnFixedUpdate(const float fixed_delta_time);
  virtual void OnRender();

  virtual void OnEvent(shared::event::Event &event);

  void SetActive(const bool active);

  [[nodiscard]] const std::string &GetName() const;
  [[nodiscard]] bool IsActive() const;

protected:
  std::string name_;
  bool active_;
};
}// namespace chroma::app::layer::state
