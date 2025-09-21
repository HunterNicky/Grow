#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "chroma/app/events/Event.h"

namespace chroma::app::event {
class EventDispatcher
{
public:
  EventDispatcher() = default;

  template<typename EventType> void Subscribe(std::function<void(EventType &)> listener)
  {
    auto wrapper = [listener](Event &event) {
      if (event.GetType() == EventType::GetStaticType()) { listener(static_cast<EventType &>(event)); }
    };
    listeners_[EventType::GetStaticType()].emplace_back(wrapper);
  }

  void Unsubscribe(Event::Type type);
  void Dispatch(Event &event);
  void Clear();

private:
  std::unordered_map<Event::Type, std::vector<std::function<void(Event &)>>> listeners_;
};
}// namespace chroma::app::event
