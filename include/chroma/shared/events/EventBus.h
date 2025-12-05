#pragma once

#include "EventDispatcher.h"

#include <memory>

namespace chroma::shared::event {

class EventBus
{
public:
  EventBus() = delete;

  static void SetDispatcher(std::unique_ptr<EventDispatcher> &dispatcher);
  [[nodiscard]] static EventDispatcher *GetDispatcher();
  static void Dispatch(Event &event);

private:
  static std::shared_ptr<EventDispatcher> event_dispatcher;
};

}// namespace chroma::shared::event
