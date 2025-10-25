#include "chroma/shared/events/MouseEvent.h"
#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {

MouseEvent::MouseEvent(Vector2 position, bool left_pressed, bool right_pressed)
    : Event(Event::MouseClickEvent), mouse_position_(position),
    left_button_pressed_(left_pressed), right_button_pressed_(right_pressed) 
{

}

MouseEvent::MouseEvent() : Event(Event::MouseEvent), mouse_position_({0.0F, 0.0F})
{

}

MouseEvent::MouseEvent(Vector2 position) : Event(Event::MouseEvent), mouse_position_(position)
{

}

Vector2 MouseEvent::GetMousePosition() const {
    return mouse_position_;
}

bool MouseEvent::IsLeftButtonPressed() const {
    return left_button_pressed_;
}

bool MouseEvent::IsRightButtonPressed() const {
    return right_button_pressed_;
}

void MouseEvent::SetMousePosition(const Vector2& position) {
    mouse_position_ = position;
}

void MouseEvent::SetLeftButtonPressed(bool pressed) {
    left_button_pressed_ = pressed;
}

void MouseEvent::SetRightButtonPressed(bool pressed) {
    right_button_pressed_ = pressed;
}

std::shared_ptr<Event> MouseEvent::Clone() const {
    return std::make_shared<MouseEvent>(*this);
}

Event::Type MouseEvent::GetStaticType() {
    return Event::MouseEvent;
}

} // namespace chroma::shared::event