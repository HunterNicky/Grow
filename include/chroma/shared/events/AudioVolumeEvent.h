#pragma once

#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {

enum class AudioChannel {
  Master = 0,
  Music = 1,
  SFX = 2,
};

class AudioVolumeEvent : public Event
{
public:
  AudioVolumeEvent(const AudioChannel channel, const float volume);

  [[nodiscard]] AudioChannel GetChannel() const;
  [[nodiscard]] float GetVolume() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  AudioChannel channel_;
  float volume_;
};
}// namespace chroma::shared::event