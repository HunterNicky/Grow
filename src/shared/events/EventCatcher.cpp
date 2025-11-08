#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::event {

void EventCatcher::CatchEvent()
{
  // Key code A ~ Z
  for (int key_code = KEY_A; key_code <= KEY_Z; ++key_code) {
    if (IsKeyPressed(key_code)) {
      KeyEvent ev = KeyEvent(key_code);
      ev.SetPressed(true);
      event_dispatcher_->Dispatch(ev);
    } else if (IsKeyReleased(key_code)) {
      KeyEvent ev = KeyEvent(key_code);
      ev.SetPressed(false);
      event_dispatcher_->Dispatch(ev);
    }
  }

  const Vector2 pos = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    MouseEvent ev = MouseEvent(pos, true, false);
    event_dispatcher_->Dispatch(ev);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    MouseEvent ev = MouseEvent(pos, false, true);
    event_dispatcher_->Dispatch(ev);
  }
  if (GetMouseDelta().x != 0 || GetMouseDelta().y != 0) {
    MouseEvent ev = MouseEvent(pos, false, false);
    event_dispatcher_->Dispatch(ev);
  }
}

void EventCatcher::SetEventDispatcher(const std::shared_ptr<EventDispatcher> &event_dispatcher)
{
  event_dispatcher_ = event_dispatcher;
}

}// namespace chroma::shared::event