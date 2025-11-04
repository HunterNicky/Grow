#include "chroma/client/audio/AudioListener.h"

namespace chroma::client::audio {

AudioListener::AudioListener() : position_({ .x = 0, .y = 0 }) {}

void AudioListener::SetPosition(const Vector2 position) { position_ = position; }

Vector2 AudioListener::GetPosition() const { return position_; }

} // namespace chroma::client::audio

