#pragma once

#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include <string>
#include <sys/types.h>
namespace chroma::shared::packet {
class InputMessage {

public:
    InputMessage() = default;
    InputMessage(uint32_t sequence, float delta_time, const std::string& player_id);
    InputMessage(const InputMessage &) = default;
    InputMessage(InputMessage &&) = delete;
    InputMessage &operator=(const InputMessage &) = default;
    InputMessage &operator=(InputMessage &&) = delete;
    ~InputMessage() = default;

    [[nodiscard]] uint32_t GetSeq() const;
    [[nodiscard]] float GetDeltaTime() const;
    [[nodiscard]] const std::string& GetPlayerId() const;

    [[nodiscard]] event::Event::Type GetEventType() const;

    [[nodiscard]] const event::KeyEvent& GetKeyEvent() const;
    [[nodiscard]] const event::MouseEvent& GetMouseEvent() const;

    void SetSeq(uint32_t sequence);
    void SetDeltaTime(float delta_time);
    void SetPlayerId(const std::string& id);

    void SetEventType(event::Event::Type type);

    void SetKeyEvent(const event::KeyEvent& key_evt);
    void SetMouseEvent(const event::MouseEvent& mouse_evt);

private:
    uint32_t seq_ {};
    float dt_ {};
    std::string player_id_;

    event::Event::Type type_ { event::Event::Type::None };

    event::KeyEvent key_event_;
    event::MouseEvent mouse_event_;
};
} // namespace chroma::shared::packet