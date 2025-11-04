#include "chroma/shared/core/components/AudioListener.h"
#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Transform.h"

#include <raylib.h>

namespace chroma::shared::core::component {

void AudioListener::Update(const float delta_time)
{
  (void)delta_time;
  const auto obj = GetGameObject();
  if (!obj) { return; }
  const auto transform = obj->GetComponent<Transform>();
  if (!transform) { return; }

  const auto bridge = audio::GetAudioBridge();
  if (bridge) {
    const Vector2 pos = transform->GetPosition();
    bridge->SetListenerPosition(pos.x, pos.y);
  }
}

}// namespace chroma::shared::core::component
