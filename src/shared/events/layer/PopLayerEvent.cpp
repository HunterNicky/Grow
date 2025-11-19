#include "chroma/shared/events/layer/PopLayerEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::layer {
PopLayerEvent::PopLayerEvent() : Event(Event::PopLayerEvent) {}

std::shared_ptr<Event> PopLayerEvent::Clone() const { return std::make_shared<PopLayerEvent>(*this); }

Event::Type PopLayerEvent::GetStaticType() { return Event::PopLayerEvent; }
}// namespace chroma::shared::event::state