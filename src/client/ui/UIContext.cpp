#include <raylib.h>

#include "chroma/client/ui/UIContext.h"

namespace chroma::client::ui {
UIContext::UIContext() : mouse_position_(Vector2{ 0, 0 }), is_left_mouse_pressed_(false) {}

Vector2 UIContext::GetMousePos() const { return mouse_position_; }

bool UIContext::GetIsLeftMousePressed() const { return is_left_mouse_pressed_; }

void UIContext::SetMousePos(Vector2 mouse_pos) { mouse_position_ = mouse_pos; }

void UIContext::SetIsLeftMousePressed(bool is_left_mouse_pressed) { is_left_mouse_pressed_ = is_left_mouse_pressed; }
}// namespace chroma::client::ui