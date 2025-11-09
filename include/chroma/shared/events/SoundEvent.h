#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>

namespace chroma::shared::event {

class SoundEvent : public Event
{
public:
  SoundEvent(std::string sound_name, float volume, float pitch);
  SoundEvent();

  [[nodiscard]] const std::string &GetSoundName() const;
  [[nodiscard]] float GetVolume() const;
  [[nodiscard]] float GetPitch() const;
  [[nodiscard]] const std::string &GetEmitterId() const { return emitter_id_; }
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

  void SetEmitterId(std::string id) { emitter_id_ = std::move(id); }

private:
  std::string sound_name_;
  float volume_;
  float pitch_;
  std::string emitter_id_;
};

}// namespace chroma::shared::event
