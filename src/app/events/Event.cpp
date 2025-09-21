#include "chroma/app/events/Event.h"

namespace chroma::app::event {
Event::Event(Event::Type type) : type_(type) {}

Event::Type Event::GetType() const { return type_; }
bool Event::IsHandled() const { return handled_; }

void Event::SetHandled(bool handled) { handled_ = handled; }
void Event::SetType(Event::Type type) { type_ = type; }
}// namespace chroma::app::event
