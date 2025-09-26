#include "chroma/app/events/EventDispatcher.h"
#include "chroma/app/events/Event.h"

namespace chroma::app::event {
void EventDispatcher::Unsubscribe(Event::Type type) { listeners_.erase(type); }

void EventDispatcher::Dispatch(Event &event)
{
  const auto iterator = listeners_.find(event.GetType());
  if (iterator != listeners_.end()) {
    for (const auto &listener : iterator->second) {
      listener(event);
      if (event.IsHandled()) { break; }
    }
  }
}

void EventDispatcher::Clear() { listeners_.clear(); }

}// namespace chroma::app::event
