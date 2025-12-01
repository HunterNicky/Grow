#include "chroma/shared/events/ShaderEvent.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event {
ShaderEvent::ShaderEvent(const ShaderEventType type)
  : Event(Type::ShaderEvent), shader_event_type_(type), pass_type_(client::render::shader::PassType::NONE)
{}

std::shared_ptr<Event> ShaderEvent::Clone() const { return std::make_shared<ShaderEvent>(*this); }

Event::Type ShaderEvent::GetStaticType() { return Type::ShaderEvent; }

ShaderEventType ShaderEvent::GetShaderEventType() const { return shader_event_type_; }

client::render::shader::PassType ShaderEvent::GetPassType() const { return pass_type_; }

void ShaderEvent::SetPassType(const client::render::shader::PassType pass_type) { pass_type_ = pass_type; }

void ShaderEvent::SetShaderEventType(const ShaderEventType type) { shader_event_type_ = type; }

bool ShaderEvent::IsFront() const { return front_; }

void ShaderEvent::SetFront(const bool front) { front_ = front; }


}// namespace chroma::shared::event