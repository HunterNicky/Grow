#include "chroma/shared/events/KeyEvent.h"

namespace chroma::shared::event {

KeyEvent::KeyEvent() : Event(Event::KeyEvent), key_(0) {}

KeyEvent::KeyEvent(uint8_t key) : Event(Event::KeyEvent), key_(key) {}

uint8_t KeyEvent::GetKey() const {
    return key_;
}

bool KeyEvent::IsPressed() const {
    return pressed_;
}

bool KeyEvent::IsReleased() const {
    return !pressed_;
}
void KeyEvent::SetPressed(bool pressed) {
    pressed_ = pressed;
}
} // namespace chroma::shared::event