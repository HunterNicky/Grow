#pragma once

#include "chroma/shared/events/Event.h"
#include "chroma/client/render/shader/RenderPass.h"
#include <cstdint>

namespace chroma::shared::event {

enum class ShaderEventType : uint8_t {
    ADD = 1,
    CHANGE = 2,
};

class ShaderEvent : public Event {
public:
    explicit ShaderEvent(ShaderEventType type);
    ~ShaderEvent() override = default;

    ShaderEvent(const ShaderEvent &) = default;
    ShaderEvent &operator=(const ShaderEvent &) = delete;
    ShaderEvent(ShaderEvent &&) noexcept = default;
    ShaderEvent &operator=(ShaderEvent &&) noexcept = default;

    [[nodiscard]] std::shared_ptr<Event> Clone() const override;
    [[nodiscard]] static Type GetStaticType();

    [[nodiscard]] bool IsFront() const;
    [[nodiscard]] ShaderEventType GetShaderEventType() const;
    [[nodiscard]] client::render::shader::PassType GetPassType() const;

    void SetPassType(client::render::shader::PassType pass_type);
    void SetShaderEventType(ShaderEventType type);
    void SetFront(bool front);

private:
    ShaderEventType shader_event_type_;
    client::render::shader::PassType pass_type_;
    bool front_{ false };
};

} // namespace chroma::shared::event