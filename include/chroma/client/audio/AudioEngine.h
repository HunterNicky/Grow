#pragma once

#include <memory>

#include "chroma/client/audio/AudioListener.h"
#include "chroma/client/audio/MusicManager.h"
#include "chroma/client/audio/SoundManager.h"

namespace chroma::client::audio {
class AudioEngine
{
public:
  AudioEngine();
  ~AudioEngine();

  SoundManager &GetSoundManager() const;
  MusicManager &GetMusicManager() const;
  AudioListener &GetAudioListener() const;

  void Update() const;

  void SetMasterVolume(float volume);

private:
  std::unique_ptr<SoundManager> sound_manager_;
  std::unique_ptr<MusicManager> music_manager_;
  std::unique_ptr<AudioListener> audio_listener_;
  float master_volume_ = 1.0f;
};
}// namespace chroma::client::audio
