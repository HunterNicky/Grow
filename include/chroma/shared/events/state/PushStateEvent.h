#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>

namespace chroma::shared::event::state {
class PushStateEvent : public Event
{
public:
  explicit PushStateEvent(std::string state_id);

  [[nodiscard]] std::string &GetStateId();
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  std::string state_id_;
};
}// namespace chroma::shared::event::state