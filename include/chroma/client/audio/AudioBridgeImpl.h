#pragma once

#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/client/audio/AudioEngine.h"

namespace chroma::client::audio {

class AudioBridgeImpl : public shared::audio::IAudioBridge
{
public:
  explicit AudioBridgeImpl(AudioEngine *engine) : engine_(engine) {}
  ~AudioBridgeImpl() override = default;

  void Update() override { if (engine_) engine_->Update(); }
  void SetMasterVolume(float volume) override { if (engine_) engine_->SetMasterVolume(volume); }

  void SetListenerPosition(const float x, const float y) override
  {
    if (engine_) engine_->GetAudioListener().SetPosition(Vector2{ x, y });
  }

  void LoadSound(const std::string &id, const std::string &file_path) override
  {
    if (engine_) engine_->GetSoundManager().LoadSound(id, file_path);
  }
  void PlaySound(const std::string &id, const float volume, const float pitch) override
  {
    if (engine_) engine_->GetSoundManager().PlaySound(id, volume, pitch);
  }
  void PlaySoundAt(const std::string &id, const float x, const float y, const float volume, const float pitch) override
  {
    if (engine_) engine_->GetSoundManager().PlaySoundAt(id, Vector2{ x, y }, volume, pitch);
  }
  void StopSound(const std::string &id) override
  {
    if (engine_) engine_->GetSoundManager().StopSound(id);
  }
  void StopAllSounds() override
  {
    if (engine_) engine_->GetSoundManager().StopAllSounds();
  }
  void UnloadSound(const std::string &id) override
  {
    if (engine_) engine_->GetSoundManager().UnloadSound(id);
  }

  void LoadMusic(const std::string &id, const std::string &file_path) override
  {
    if (engine_) engine_->GetMusicManager().LoadMusic(id, file_path);
  }
  void PlayMusic(const std::string &id, const bool loop, const float volume) override
  {
    if (engine_) engine_->GetMusicManager().PlayMusic(id, loop, volume);
  }
  void StopMusic() override
  {
    if (engine_) engine_->GetMusicManager().StopMusic();
  }
  void UnloadMusic(const std::string &id) override
  {
    if (engine_) engine_->GetMusicManager().UnloadMusic(id);
  }
  [[nodiscard]] std::optional<std::string> GetCurrentMusic() const override
  {
    if (engine_) return engine_->GetMusicManager().GetCurrentMusic();
    return std::nullopt;
  }

private:
  AudioEngine *engine_;
};

} // namespace chroma::client::audio
