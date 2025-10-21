#pragma once
#include "chroma/app/events/Event.h"
#include <cstdint>

namespace chroma::app::event {

class KeyEvent : public Event {
public:
    explicit KeyEvent(uint8_t key) : Event(Event::KeyEvent), key_(key), pressed_(false) {}

    [[nodiscard]] uint8_t GetKey() const { return key_; }
    [[nodiscard]] bool IsPressed() const { return pressed_; }
    [[nodiscard]] bool IsReleased() const { return !pressed_; }

    void SetPressed(bool pressed) { pressed_ = pressed; }

private:
    uint8_t key_;
    // NOLINTNEXTLINE
    bool pressed_;
};

} // namespace chroma::app::event
