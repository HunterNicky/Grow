#pragma once
#include "chroma/app/events/Event.h"
#include <cstdint>

namespace chroma::app::event {

class KeyPressedEvent : public Event {
public:
    explicit KeyPressedEvent(uint8_t key) : Event(Event::KeyPressed), key_(key) {}

    [[nodiscard]] uint8_t GetKey() const { return key_; }

private:
    uint8_t key_;
};

} // namespace chroma::app::event
