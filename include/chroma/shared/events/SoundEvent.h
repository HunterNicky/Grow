#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>

namespace chroma::shared::event {

class SoundEvent : public Event
{
public:
  SoundEvent(const std::string &sound_name, float volume, float pitch);
  SoundEvent();

  [[nodiscard]] const std::string &GetSoundName() const;
  [[nodiscard]] float GetVolume() const;
  [[nodiscard]] float GetPitch() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  std::string sound_name_;
  float volume_;
  float pitch_;
};

}// namespace chroma::shared::event
