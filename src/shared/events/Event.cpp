#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {
Event::Event(Type type) : type_(type) {}

Event::Type Event::GetType() const { return type_; }
bool Event::IsHandled() const { return handled_; }

void Event::SetHandled(bool handled) { handled_ = handled; }
void Event::SetType(Type type) { type_ = type; }
}// namespace chroma::shared::event
