#pragma once

#include <memory>
#include <optional>
#include <string>

namespace chroma::shared::audio {

class IAudioBridge
{
public:
  virtual ~IAudioBridge() = default;

  virtual void Update() = 0;
  virtual void SetMasterVolume(float volume) = 0;

  virtual void SetListenerPosition(float x, float y) = 0;

  virtual void LoadSound(const std::string &id, const std::string &file_path) = 0;
  virtual void PlaySound(const std::string &id, float volume = 1.0F, float pitch = 1.0F) = 0;
  virtual void PlaySoundAt(const std::string &id, float x, float y, float volume = 1.0F, float pitch = 1.0F) = 0;
  virtual void StopSound(const std::string &id) = 0;
  virtual void StopAllSounds() = 0;
  virtual void UnloadSound(const std::string &id) = 0;

  virtual void LoadMusic(const std::string &id, const std::string &file_path) = 0;
  virtual void PlayMusic(const std::string &id, bool loop = true, float volume = 1.0f) = 0;
  virtual void StopMusic() = 0;
  virtual void UnloadMusic(const std::string &id) = 0;
  [[nodiscard]] virtual std::optional<std::string> GetCurrentMusic() const = 0;
};

void SetAudioBridge(std::shared_ptr<IAudioBridge> bridge);
std::shared_ptr<IAudioBridge> GetAudioBridge();

}// namespace chroma::shared::audio
