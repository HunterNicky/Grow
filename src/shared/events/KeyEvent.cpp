#include "chroma/shared/events/KeyEvent.h"

namespace chroma::shared::event {

KeyEvent::KeyEvent(uint8_t key) : Event(Event::KeyEvent), key_(key) {}

KeyEvent::KeyEvent() : Event(Event::KeyEvent), key_(0) {}

uint8_t KeyEvent::GetKey() const { return key_; }

bool KeyEvent::IsPressed() const { return pressed_; }

bool KeyEvent::IsReleased() const { return !pressed_; }
void KeyEvent::SetPressed(bool pressed) { pressed_ = pressed; }

std::shared_ptr<Event> KeyEvent::Clone() const { return std::make_shared<KeyEvent>(*this); }

Event::Type KeyEvent::GetStaticType() { return Event::KeyEvent; }

}// namespace chroma::shared::event