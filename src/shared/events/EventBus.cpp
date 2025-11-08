#include "chroma/shared/events/EventBus.h"

#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventDispatcher.h"

#include <memory>
#include <utility>

namespace chroma::shared::event {
std::unique_ptr<EventDispatcher> EventBus::event_dispatcher = nullptr;

void EventBus::SetDispatcher(std::unique_ptr<EventDispatcher> &dispatcher)
{
  if (!event_dispatcher) { event_dispatcher = std::move(dispatcher); }
}

EventDispatcher *EventBus::GetDispatcher()
{
  if (event_dispatcher) { return event_dispatcher.get(); }
  return nullptr;
}

void EventBus::Dispatch(Event &event)
{
  if (event_dispatcher) { event_dispatcher->Dispatch(event); }
}
}// namespace chroma::shared::event
