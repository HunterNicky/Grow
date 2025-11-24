#include "chroma/shared/events/AudioVolumeEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event {
AudioVolumeEvent::AudioVolumeEvent(const AudioChannel channel, const float volume) : Event(Type::AudioVolumeEvent), channel_(channel), volume_(volume)
{}

AudioChannel AudioVolumeEvent::GetChannel() const { return channel_; }
float AudioVolumeEvent::GetVolume() const { return volume_; }
std::shared_ptr<Event> AudioVolumeEvent::Clone() const { return std::make_shared<AudioVolumeEvent>(*this); }

Event::Type AudioVolumeEvent::GetStaticType() { return Event::AudioVolumeEvent; }
}// namespace chroma::shared::event