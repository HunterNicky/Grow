#include "chroma/shared/events/InputState.h"

namespace chroma::shared::events {

void InputState::SetKeyState(int key_code, bool pressed) {
    key_states_[key_code] = pressed;
}

bool InputState::IsKeyPressed(int key_code) const {
    auto it = key_states_.find(key_code);
    if (it != key_states_.end()) {
        return it->second;
    }
    return false;
}
}// namespace chroma::shared::events