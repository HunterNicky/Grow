#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include "chroma/shared/events/EventDispatcher.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::event {

void EventCatcher::CatchEvent()
{
  // Key code A ~ Z
  for (int key_code = 65; key_code <= 90; ++key_code) {
    if (IsKeyPressed(key_code)) {
      event::KeyEvent ev = event::KeyEvent(key_code);
      ev.SetPressed(true);
      event_dispatcher_->Dispatch(ev);
      return;
    } else if (IsKeyReleased(key_code)) {
      event::KeyEvent ev = event::KeyEvent(key_code);
      ev.SetPressed(false);
      event_dispatcher_->Dispatch(ev);
      return;
    }
  }

  const Vector2 pos = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    event::MouseEvent ev = event::MouseEvent(pos, true, false);
    event_dispatcher_->Dispatch(ev);
    return;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    event::MouseEvent ev = event::MouseEvent(pos, false, true);
    event_dispatcher_->Dispatch(ev);
    return;
  }
  if (GetMouseDelta().x != 0 || GetMouseDelta().y != 0) {
    event::MouseEvent ev = event::MouseEvent(pos, false, false);
    event_dispatcher_->Dispatch(ev);
    return;
  }
}

void EventCatcher::SetEventDispatcher(const std::shared_ptr<chroma::shared::event::EventDispatcher> &event_dispatcher)
{
  event_dispatcher_ = event_dispatcher;
}

}// namespace chroma::shared::event