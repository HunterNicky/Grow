#include "chroma/shared/events/layer/LayerEvent.h"
#include "chroma/app/layers/LayerIdentifiers.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::layer {
LayerEvent::LayerEvent(const Action action) : Event(Type::LayerEvent), action_(action), layer_id_(app::layer::LayerID::None) {}
LayerEvent::LayerEvent(const Action action, const app::layer::LayerID layer_id) : Event(Type::LayerEvent), action_(action), layer_id_(layer_id)
{}

app::layer::LayerID LayerEvent::GetLayerId() const { return layer_id_; }
Action LayerEvent::GetAction() const { return action_; }
std::shared_ptr<Event> LayerEvent::Clone() const { return std::make_shared<LayerEvent>(*this); }

Event::Type LayerEvent::GetStaticType() { return Event::LayerEvent; }
}// namespace chroma::shared::event::layer