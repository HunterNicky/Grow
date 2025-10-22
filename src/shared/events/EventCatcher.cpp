#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::event {

std::unique_ptr<event::Event> EventCatcher::CatchEvent() {
    
    if (IsKeyPressed(KEY_W)) {
        return std::make_unique<event::KeyEvent>(KEY_W);
    }
    if (IsKeyReleased(KEY_W)) {
        auto ev = std::make_unique<event::KeyEvent>(KEY_W);
        ev->SetPressed(false);
        return ev;
    }
    
    Vector2 pos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<event::MouseEvent>(pos, true, false);
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        return std::make_unique<event::MouseEvent>(pos, false, true);
    }
    if (GetMouseDelta().x != 0 || GetMouseDelta().y != 0) {
        return std::make_unique<event::MouseEvent>(pos);
    }
    return nullptr;
}
} // namespace chroma::shared::event