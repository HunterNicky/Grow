#pragma once

#include <raylib.h>

namespace chroma::client::ui {
class UIContext
{
public:
  UIContext();
  ~UIContext() = default;
  [[nodiscard]] Vector2 GetMousePos() const;
  [[nodiscard]] bool GetIsLeftMousePressed() const;
  [[nodiscard]] void SetMousePos(Vector2 mouse_pos);
  [[nodiscard]] void SetIsLeftMousePressed(bool is_left_mouse_pressed);

private:
  Vector2 mouse_position_;
  bool is_left_mouse_pressed_;
};
}