#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {
void EventDispatcher::Unsubscribe(Event::Type type) { listeners_.erase(type); }

void EventDispatcher::Dispatch(Event &event)
{
  if(chroma::shared::event::Event::Type::None == event.GetType()) { return; }

  const auto iterator = listeners_.find(event.GetType());
  if (iterator != listeners_.end()) {
    for (const auto &listener : iterator->second) {
      listener(event);
      if (event.IsHandled()) { break; }
    }
  }
}

void EventDispatcher::Clear() { listeners_.clear(); }

}// namespace chroma::shared::event
