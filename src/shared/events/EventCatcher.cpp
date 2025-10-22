#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::event {

std::unique_ptr<event::Event> EventCatcher::CatchEvent() {
    //Key code A a Z
    for (int key_code = 65; key_code <= 90; ++key_code) {
        auto key_event = VerifyKeyEvent(key_code);
        if (key_event != nullptr) {
            return key_event;
        }
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

std::unique_ptr<event::Event> EventCatcher::VerifyKeyEvent(int key_code) {
    if (IsKeyPressed(key_code)) {
        return std::make_unique<event::KeyEvent>(key_code);
    }
    if (IsKeyReleased(key_code)) {
        auto ev = std::make_unique<event::KeyEvent>(key_code);
        ev->SetPressed(false);
        return ev;
    }
    return nullptr;
}
} // namespace chroma::shared::event