#pragma once

#include "chroma/shared/events/Event.h"

#include <cstdint>
#include <memory>

namespace chroma::shared::event {

class WaveEvent final : public Event
{
public:
  explicit WaveEvent(uint32_t wave_index) : Event(Type::WaveEvent), wave_index_(wave_index) {}

  [[nodiscard]] uint32_t GetWaveIndex() const { return wave_index_; }

  [[nodiscard]] std::shared_ptr<Event> Clone() const override
  {
    return std::make_shared<WaveEvent>(*this);
  }

  [[nodiscard]] static Type GetStaticType() { return Type::WaveEvent; }

private:
  uint32_t wave_index_;
};

}// namespace chroma::shared::event

