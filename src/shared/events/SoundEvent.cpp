#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>
#include <utility>

namespace chroma::shared::event {
SoundEvent::SoundEvent(std::string sound_name, const float volume, const float pitch)
  : Event(Type::SoundEvent), sound_name_(std::move(sound_name)), volume_(volume), pitch_(pitch)
{}

SoundEvent::SoundEvent() : Event(Type::SoundEvent), volume_(1.0F), pitch_(1.0F) {}

const std::string &SoundEvent::GetSoundName() const { return sound_name_; }

float SoundEvent::GetVolume() const { return volume_; }

float SoundEvent::GetPitch() const { return pitch_; }

std::shared_ptr<Event> SoundEvent::Clone() const { return std::make_shared<SoundEvent>(*this); }

Event::Type SoundEvent::GetStaticType() { return Event::SoundEvent; }

}// namespace chroma::shared::event