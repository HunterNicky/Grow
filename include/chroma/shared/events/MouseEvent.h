#pragma once

#include "chroma/shared/events/Event.h"

#include <raylib.h>

namespace chroma::shared::event {
class MouseEvent : public Event {
public:
    explicit MouseEvent(Vector2 position);
    MouseEvent(Vector2 position, bool left_pressed, bool right_pressed);
    MouseEvent();
    
    [[nodiscard]] Vector2 GetMousePosition() const;
    [[nodiscard]] bool IsLeftButtonPressed() const;
    [[nodiscard]] bool IsRightButtonPressed() const;
    [[nodiscard]] std::shared_ptr<Event> Clone() const override;

    void SetMousePosition(const Vector2& position);
    void SetLeftButtonPressed(bool pressed);
    void SetRightButtonPressed(bool pressed);

private:
    Vector2 mouse_position_ = {0.0F, 0.0F};
    bool left_button_pressed_ = false;
    bool right_button_pressed_ = false;
};
} // namespace chroma::shared::event