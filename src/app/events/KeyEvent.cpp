#include "chroma/app/events/KeyEvent.h"

namespace chroma::app::event {

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
} // namespace chroma::app::event