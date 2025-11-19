#include "chroma/shared/events/state/PopStateEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::state {
PopStateEvent::PopStateEvent() : Event(Event::PopStateEvent) {}

std::shared_ptr<Event> PopStateEvent::Clone() const { return std::make_shared<PopStateEvent>(*this); }

Event::Type PopStateEvent::GetStaticType() { return Event::PopStateEvent; }
}// namespace chroma::shared::event::state