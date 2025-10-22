#pragma once

#include "chroma/shared/events/Event.h"

#include <cstdint>

namespace chroma::shared::event {

class KeyEvent : public Event {
public:
    explicit KeyEvent(uint8_t key);

    [[nodiscard]] uint8_t GetKey() const;
    [[nodiscard]] bool IsPressed() const;
    [[nodiscard]] bool IsReleased() const;

    void SetPressed(bool pressed);

private:
    uint8_t key_;
    bool pressed_ = false;
};

} // namespace chroma::shared::event
