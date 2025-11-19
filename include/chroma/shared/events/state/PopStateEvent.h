#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::state {
class PopStateEvent : public Event
{
public:
  PopStateEvent();

  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();
};
}// namespace chroma::shared::event::state