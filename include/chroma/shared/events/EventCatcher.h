#pragma once

#include "chroma/shared/events/EventDispatcher.h"

#include <memory>

namespace chroma::shared::event {
class EventCatcher
{
public:
  void CatchEvent();
  void SetEventDispatcher(const std::shared_ptr<chroma::shared::event::EventDispatcher> &event_dispatcher);

private:
  std::shared_ptr<chroma::shared::event::EventDispatcher> event_dispatcher_;
};
}// namespace chroma::shared::event