#pragma once

#include "chroma/shared/events/Event.h"

#include <sys/types.h>

namespace chroma::shared::packet {
class InputMessage {

public:
    InputMessage() = default;
    InputMessage(uint32_t sequence, float delta_time);
    InputMessage(const InputMessage &) = default;
    InputMessage(InputMessage &&) = delete;
    InputMessage &operator=(const InputMessage &) = default;
    InputMessage &operator=(InputMessage &&) = delete;
    ~InputMessage() = default;

    [[nodiscard]] uint32_t GetSeq() const;
    [[nodiscard]] float GetDeltaTime() const;

    [[nodiscard]] event::Event::Type GetEventType() const;

    [[nodiscard]] const event::Event& GetEvent() const;

    void SetSeq(uint32_t sequence);
    void SetDeltaTime(float delta_time);

    void SetEventType(event::Event::Type type);

    void SetEvent(const event::Event& event);

private:
    uint32_t seq_ {};
    float dt_ {};

    event::Event::Type type_ { event::Event::Type::None };

    event::Event event_;
};
} // namespace chroma::shared::packet