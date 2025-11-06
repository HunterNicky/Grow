#include "chroma/shared/events/EventBus.h"

#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventDispatcher.h"

#include <memory>
#include <utility>

namespace chroma::shared::event {
namespace {
  std::weak_ptr<EventDispatcher> &DispatcherStorage()
  {
    static std::weak_ptr<EventDispatcher> dispatcher;
    return dispatcher;
  }
}// namespace

void EventBus::SetDispatcher(std::shared_ptr<EventDispatcher> dispatcher)
{
  DispatcherStorage() = std::move(dispatcher);
}

std::shared_ptr<EventDispatcher> EventBus::GetDispatcher() { return DispatcherStorage().lock(); }

void EventBus::Dispatch(Event &event)
{
  if (auto dispatcher = DispatcherStorage().lock()) { dispatcher->Dispatch(event); }
}

}// namespace chroma::shared::event
