#pragma once

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>

namespace chroma::shared::core::component {

class AudioListener final : public Component
{
public:
  AudioListener() { type_ = ComponentType::AUDIO_LISTENER; }
  ~AudioListener() override = default;

  void Update(float delta_time) override;
};

} // namespace chroma::shared::core::component

