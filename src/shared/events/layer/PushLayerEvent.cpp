#include "chroma/shared/events/layer/PushLayerEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>
#include <utility>

namespace chroma::shared::event::layer {
PushLayerEvent::PushLayerEvent(std::string layer_id) : Event(Event::PushLayerEvent), layer_id_(std::move(layer_id)) {}

std::string &PushLayerEvent::GetLayerId() { return layer_id_; }

std::shared_ptr<Event> PushLayerEvent::Clone() const { return std::make_shared<PushLayerEvent>(*this); }

Event::Type PushLayerEvent::GetStaticType() { return Event::PushLayerEvent; }
}// namespace chroma::shared::event::state