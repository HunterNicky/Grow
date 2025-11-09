#include "chroma/shared/events/EventCatcher.h"

#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"

#include <raylib.h>

namespace chroma::shared::event {

void EventCatcher::CatchEvent()
{
  // Key code A ~ Z
  for (int key_code = KEY_A; key_code <= KEY_Z; ++key_code) {
    if (IsKeyPressed(key_code)) {
      KeyEvent ev = KeyEvent(key_code);
      ev.SetPressed(true);
      EventBus::GetDispatcher()->Dispatch(ev);
    } else if (IsKeyReleased(key_code)) {
      KeyEvent ev = KeyEvent(key_code);
      ev.SetPressed(false);
      EventBus::GetDispatcher()->Dispatch(ev);
    }
  }

  const Vector2 pos = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    MouseEvent ev = MouseEvent(pos, true, false);
    EventBus::GetDispatcher()->Dispatch(ev);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    MouseEvent ev = MouseEvent(pos, false, true);
    EventBus::GetDispatcher()->Dispatch(ev);
  }
  if (GetMouseDelta().x != 0 || GetMouseDelta().y != 0) {
    MouseEvent ev = MouseEvent(pos, false, false);
    EventBus::GetDispatcher()->Dispatch(ev);
  }
}

}// namespace chroma::shared::event