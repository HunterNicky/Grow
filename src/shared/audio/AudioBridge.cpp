#include "chroma/shared/audio/AudioBridge.h"

#include <memory>
#include <utility>

namespace chroma::shared::audio {

static std::shared_ptr<IAudioBridge> &BridgeStorage()
{
  static std::shared_ptr<IAudioBridge> s_bridge;
  return s_bridge;
}

void SetAudioBridge(std::shared_ptr<IAudioBridge> bridge) { BridgeStorage() = std::move(bridge); }

std::shared_ptr<IAudioBridge> GetAudioBridge() { return BridgeStorage(); }

} // namespace chroma::shared::audio
