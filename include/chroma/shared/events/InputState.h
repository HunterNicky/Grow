#pragma once

#include <unordered_map>

namespace chroma::shared::events {
class InputState {
public:
    InputState() = default;
    InputState(const InputState&) = default;
    InputState& operator=(const InputState&) = default;
    InputState(InputState&&) noexcept = default;
    InputState& operator=(InputState&&) noexcept = default;
    ~InputState() = default;

    void SetKeyState(int key_code, bool pressed);
    [[nodiscard]] bool IsKeyPressed(int key_code) const;

    private:
    std::unordered_map<int, bool> key_states_;
};
}// namespace chroma::shared::events