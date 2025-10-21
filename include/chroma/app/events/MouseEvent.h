#include "chroma/app/events/Event.h"

#include <raymath.h>

namespace chroma::app::event {
class MouseEvent : public Event {
public:
    explicit MouseEvent(Vector2 position) : Event(Event::MouseEvent), mousePosition_(position), leftButtonPressed_(false), rightButtonPressed_(false) {}
    MouseEvent(Vector2 position, bool leftPressed, bool rightPressed)
        : Event(Event::MouseEvent), mousePosition_(position), leftButtonPressed_(leftPressed), rightButtonPressed_(rightPressed) {}

    [[nodiscard]] Vector2 GetMousePosition() const { return mousePosition_; }
    [[nodiscard]] bool IsLeftButtonPressed() const { return leftButtonPressed_; }
    [[nodiscard]] bool IsRightButtonPressed() const { return rightButtonPressed_; }

    void SetMousePosition(const Vector2& position) { mousePosition_ = position; }
    void SetLeftButtonPressed(bool pressed) { leftButtonPressed_ = pressed; }
    void SetRightButtonPressed(bool pressed) { rightButtonPressed_ = pressed; }

private:
    Vector2 mousePosition_;
    // NOLINTNEXTLINE
    bool leftButtonPressed_;

    // NOLINTNEXTLINE
    bool rightButtonPressed_;
};
} // namespace chroma::app::event