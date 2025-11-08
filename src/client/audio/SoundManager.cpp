#include "chroma/client/audio/SoundManager.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>

namespace chroma::client::audio {

SoundManager::~SoundManager()
{
  for (const auto &inst : active_) {
    ::StopSound(inst.sound);
    UnloadSoundAlias(inst.sound);
  }
  active_.clear();

  for (const auto &pair : sounds_) { ::UnloadSound(pair.second); }
  sounds_.clear();
}

void SoundManager::LoadSound(const std::string &id, const std::string &file_path)
{
  if (sounds_.contains(id)) { return; }
  const Sound sound = ::LoadSound(file_path.c_str());
  sounds_[id] = sound;
}

void SoundManager::PlaySound(const std::string &id, const float volume, const float pitch)
{
  const auto it = sounds_.find(id);
  if (it == sounds_.end()) { return; }

  const Sound alias = LoadSoundAlias(it->second);
  SetSoundVolume(alias, volume);
  SetSoundPitch(alias, pitch);
  ::PlaySound(alias);

  ActiveInstance inst{};
  inst.sound = alias;
  inst.base_id = id;
  inst.position = { .x = 0, .y = 0 };
  inst.base_volume = volume;
  active_.push_back(inst);
}

void SoundManager::PlaySoundAt(const std::string &id, const Vector2 position, const float volume, const float pitch)
{
  const auto it = sounds_.find(id);
  if (it == sounds_.end()) { return; }

  const Sound alias = LoadSoundAlias(it->second);
  SetSoundPitch(alias, pitch);

  constexpr float pan_range = 500.0F;
  constexpr float ref_distance = 256.0F;
  constexpr float max_distance = 2000.0F;

  const Vector2 delta = Vector2Subtract(position, listener_position_);
  float distance = Vector2Length(delta);
  distance = std::min(distance, max_distance);

  const float pan_x = std::clamp(delta.x / pan_range, -1.0F, 1.0F);
  const float pan = 0.5F - (pan_x * 0.5F);
  const float attenuation = std::pow(ref_distance / (ref_distance + distance), 2.0F);
  const float final_volume = std::clamp(volume * attenuation, 0.0F, 1.0F);

  SetSoundPan(alias, pan);
  SetSoundVolume(alias, final_volume);

  ::PlaySound(alias);

  ActiveInstance inst{};
  inst.sound = alias;
  inst.base_id = id;
  inst.position = position;
  inst.base_volume = volume;
  active_.push_back(inst);
}

void SoundManager::StopSound(const std::string &id)
{
  const auto it = sounds_.find(id);
  if (it != sounds_.end()) { ::StopSound(it->second); }

  for (auto &inst : active_) {
    if (inst.base_id == id) { ::StopSound(inst.sound); }
  }
}

void SoundManager::StopAllSounds() const
{
  for (const auto &pair : sounds_) { ::StopSound(pair.second); }
  for (const auto &inst : active_) { ::StopSound(inst.sound); }
}

void SoundManager::UnloadSound(const std::string &id)
{
  std::erase_if(active_, [id](const auto &entry) {
    if (entry.base_id == id) {
      ::StopSound(entry.sound);
      UnloadSoundAlias(entry.sound);
      return true;
    }
    return false;
  });

  const auto it = sounds_.find(id);
  if (it != sounds_.end()) {
    ::UnloadSound(it->second);
    sounds_.erase(it);
  }
}

void SoundManager::UpdateSpatial(const Vector2 listener_position)
{
  listener_position_ = listener_position;

  std::erase_if(active_, [](const ActiveInstance &a) {
    if (!IsSoundPlaying(a.sound)) {
      UnloadSoundAlias(a.sound);
      return true;
    }
    return false;
  });
}

}// namespace chroma::client::audio
