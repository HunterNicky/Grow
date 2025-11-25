#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

#include <map>

namespace chroma::shared::event {

EventDispatcher::EventDispatcher() : next_listener_id_(0) {}

void EventDispatcher::Unsubscribe(const SubscriptionInfo &info)
{
  if (info.id_ == 0) { return; }

  auto it = listeners_.find(info.event_type_);
  if (it != listeners_.end()) { it->second.erase(info.id_); }
}

void EventDispatcher::Dispatch(Event &event)
{
  if (Event::Type::None == event.GetType()) { return; }

  auto it = listeners_.find(event.GetType());
  if (it != listeners_.end()) {
    for (const auto &pair : it->second) {
      pair.second(event);
      if (event.IsHandled()) { break; };
    }
  }
}

void EventDispatcher::Clear() { listeners_.clear(); }

}// namespace chroma::shared::event
