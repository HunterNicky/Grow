#include "chroma/shared/packet/InputMessage.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <cstdint>

namespace chroma::shared::packet {
InputMessage::InputMessage(uint32_t sequence, float delta_time) : seq_(sequence), dt_(delta_time) {}

uint32_t InputMessage::GetSeq() const { return seq_; }

float InputMessage::GetDeltaTime() const { return dt_; }


event::Event::Type InputMessage::GetEventType() const { return type_; }

const std::shared_ptr<event::Event> &InputMessage::GetEvent() const { return event_; }

void InputMessage::SetSeq(uint32_t sequence) { seq_ = sequence; }

void InputMessage::SetDeltaTime(float delta_time) { dt_ = delta_time; }

void InputMessage::SetEventType(event::Event::Type type) { type_ = type; }

void InputMessage::SetEvent(const event::Event &event) { event_ = event.Clone(); }
}// namespace chroma::shared::packet