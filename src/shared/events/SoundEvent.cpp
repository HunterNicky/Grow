#include "chroma/shared/events/SoundEvent.h"

#include <utility>

namespace chroma::shared::event {

SoundEvent::SoundEvent(const std::string &sound_name, float volume, float pitch)
  : Event(Type::SoundEvent), sound_name_(std::move(sound_name)), volume_(volume), pitch_(pitch)
{}

SoundEvent::SoundEvent() : Event(Type::SoundEvent), sound_name_(""), volume_(1.0F), pitch_(1.0F) {}

const std::string &SoundEvent::GetSoundName() const { return sound_name_; }

float SoundEvent::GetVolume() const { return volume_; }

float SoundEvent::GetPitch() const { return pitch_; }

std::shared_ptr<Event> SoundEvent::Clone() const { return std::make_shared<SoundEvent>(*this); }

Event::Type SoundEvent::GetStaticType() { return Event::SoundEvent; }

} // namespace event::shared::chroma
