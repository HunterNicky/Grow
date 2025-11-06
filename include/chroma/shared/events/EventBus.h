#pragma once

#include <memory>

namespace chroma::shared::event {

class Event;
class EventDispatcher;

class EventBus
{
public:
  EventBus() = delete;

  static void SetDispatcher(std::shared_ptr<EventDispatcher> dispatcher);
  [[nodiscard]] static std::shared_ptr<EventDispatcher> GetDispatcher();
  static void Dispatch(Event &event);
};

} // namespace event::namespace shared::namespace chroma
