#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace chroma::client::audio {

class SoundManager
{
public:
  SoundManager() = default;
  ~SoundManager();

  void LoadSound(const std::string &id, const std::string &file_path);
  void PlaySound(const std::string &id, float volume = 1.0f, float pitch = 1.0f);
  void PlaySoundAt(const std::string &id, Vector2 position, float volume = 1.0f, float pitch = 1.0f);

  void StopSound(const std::string &id);
  void StopAllSounds() const;
  void UnloadSound(const std::string &id);

  void UpdateSpatial(Vector2 listener_position);

private:
  struct ActiveInstance
  {
    Sound sound;
    std::string base_id;
    Vector2 position;
    float base_volume = 1.0F;
  };

  std::unordered_map<std::string, Sound> sounds_;
  Vector2 listener_position_{ 0, 0 };
  std::vector<ActiveInstance> active_;
};

}// namespace chroma::client::audio
