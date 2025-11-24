#include "chroma/shared/events/ShaderEvent.h"
#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {
ShaderEvent::ShaderEvent(ShaderEventType type)
    : Event(Event::Type::ShaderEvent), shader_event_type_(type), pass_type_(client::render::shader::PassType::NONE)
{
}

std::shared_ptr<Event> ShaderEvent::Clone() const
{
    return std::make_shared<ShaderEvent>(*this);
}

Event::Type ShaderEvent::GetStaticType()
{
    return Event::Type::ShaderEvent;
}

ShaderEventType ShaderEvent::GetShaderEventType() const
{
    return shader_event_type_;
}

client::render::shader::PassType ShaderEvent::GetPassType() const
{
    return pass_type_;
}

void ShaderEvent::SetPassType(client::render::shader::PassType pass_type)
{
    pass_type_ = pass_type;
}

void ShaderEvent::SetShaderEventType(ShaderEventType type)
{
    shader_event_type_ = type;
}

bool ShaderEvent::IsFront() const
{
    return front_;
}

void ShaderEvent::SetFront(bool front)
{
    front_ = front;
}


} // namespace chroma::shared::event