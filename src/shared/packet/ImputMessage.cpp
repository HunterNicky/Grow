#include "chroma/shared/packet/ImputMessage.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include <cstdint>

namespace chroma::shared::packet {
InputMessage::InputMessage(uint32_t sequence, float delta_time, const std::string& player_id)
: seq_(sequence), dt_(delta_time), player_id_(player_id)
{
    key_event_ = event::KeyEvent();
    mouse_event_ = event::MouseEvent();
}

uint32_t InputMessage::GetSeq() const {
    return seq_;
}

float InputMessage::GetDeltaTime() const {
    return dt_;
}

const std::string& InputMessage::GetPlayerId() const {
    return player_id_;
}

event::Event::Type InputMessage::GetEventType() const {
    return type_;
}

const event::KeyEvent& InputMessage::GetKeyEvent() const {
    return key_event_;
}

const event::MouseEvent& InputMessage::GetMouseEvent() const {
    return mouse_event_;
}

void InputMessage::SetSeq(uint32_t sequence) {
    seq_ = sequence;
}

void InputMessage::SetDeltaTime(float delta_time) {
    dt_ = delta_time;
}

void InputMessage::SetPlayerId(const std::string& id) {
    player_id_ = id;
}

void InputMessage::SetEventType(event::Event::Type type) {
    type_ = type;
}

void InputMessage::SetKeyEvent(const event::KeyEvent& key_evt) {
    key_event_ = key_evt;
}

void InputMessage::SetMouseEvent(const event::MouseEvent& mouse_evt) {
    mouse_event_ = mouse_evt;
}

}