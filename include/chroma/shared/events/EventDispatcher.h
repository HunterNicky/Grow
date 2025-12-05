#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <unordered_map>
#include <memory>

#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::shared::event {
class EventDispatcher : public std::enable_shared_from_this<EventDispatcher>
{
public:
  EventDispatcher();
  ~EventDispatcher();

  template<typename EventType> Subscription Subscribe(std::function<void(EventType &)> listener)
  {
    auto wrapper = [listener](Event &event) {
      if (event.GetType() == EventType::GetStaticType()) { listener(static_cast<EventType &>(event)); }
    };

    SubscriptionInfo info = { EventType::GetStaticType(), ++next_listener_id_ };
    info.dispatcher_ = this->shared_from_this();

    listeners_[info.event_type_].emplace(info.id_, wrapper);
    return Subscription(info);
  }

  void Unsubscribe(const SubscriptionInfo& info);
  void Dispatch(Event &event);
  void Clear();

private:
  uint64_t next_listener_id_;
  std::unordered_map<Event::Type, std::map<uint64_t, std::function<void(Event &)>>> listeners_;
};
}// namespace chroma::shared::event
