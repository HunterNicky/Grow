#include "chroma/client/audio/AudioEngine.h"
#include "chroma/client/audio/AudioListener.h"
#include "chroma/client/audio/MusicManager.h"
#include "chroma/client/audio/SoundManager.h"

#include <memory>
#include <raylib.h>
#include <raymath.h>

namespace chroma::client::audio {

AudioEngine::AudioEngine()
{
  InitAudioDevice();
  sound_manager_ = std::make_unique<SoundManager>();
  music_manager_ = std::make_unique<MusicManager>();
  audio_listener_ = std::make_unique<AudioListener>();
}

AudioEngine::~AudioEngine() { CloseAudioDevice(); }

SoundManager &AudioEngine::GetSoundManager() const { return *sound_manager_; }
MusicManager &AudioEngine::GetMusicManager() const { return *music_manager_; }
AudioListener &AudioEngine::GetAudioListener() const { return *audio_listener_; }

void AudioEngine::Update() const
{
  music_manager_->Update();
  if (sound_manager_ && audio_listener_) { sound_manager_->UpdateSpatial(audio_listener_->GetPosition()); }
}

void AudioEngine::SetMasterVolume(const float volume)
{
  const float clamped = Clamp(volume, 0.0F, 1.0F);
  this->master_volume_ = clamped;
  ::SetMasterVolume(clamped);
}

}// namespace chroma::client::audio
