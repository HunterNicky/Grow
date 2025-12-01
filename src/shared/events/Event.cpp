#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {
Event::Event(const Type type) : type_(type) {}

Event::Type Event::GetType() const { return type_; }
bool Event::IsHandled() const { return handled_; }

void Event::SetHandled(const bool handled) { handled_ = handled; }
void Event::SetType(const Type type) { type_ = type; }
}// namespace chroma::shared::event
