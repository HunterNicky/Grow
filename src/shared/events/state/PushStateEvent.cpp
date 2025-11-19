#include "chroma/shared/events/state/PushStateEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>
#include <utility>

namespace chroma::shared::event::state {
PushStateEvent::PushStateEvent(std::string state_id) : Event(Event::PushStateEvent), state_id_(std::move(state_id)) {}

std::string &PushStateEvent::GetStateId() { return state_id_; }

std::shared_ptr<Event> PushStateEvent::Clone() const { return std::make_shared<PushStateEvent>(*this); }

Event::Type PushStateEvent::GetStaticType() { return Event::PushStateEvent; }
}// namespace chroma::shared::event::state