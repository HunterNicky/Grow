#include "chroma/client/audio/SoundManager.h"

#include <algorithm>
#include <cmath>
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
  if (it != sounds_.end()) {
    const Sound alias = LoadSoundAlias(it->second);
    SetSoundVolume(alias, volume);
    SetSoundPitch(alias, pitch);
    SetSoundPan(alias, 0.5F);
    ::PlaySound(alias);

    ActiveInstance inst{};
    inst.sound = alias;
    inst.base_id = id;
    inst.position = { .x = 0, .y = 0 };
    inst.base_volume = volume;
    active_.push_back(inst);
  }
}

void SoundManager::PlaySoundAt(const std::string &id, const Vector2 position, const float volume, const float pitch)
{
  const auto it = sounds_.find(id);
  if (it != sounds_.end()) {
    const Sound alias = LoadSoundAlias(it->second);
    SetSoundVolume(alias, volume);
    SetSoundPitch(alias, pitch);
    SetSoundPan(alias, 0.5F);
    ::PlaySound(alias);

    ActiveInstance inst{};
    inst.sound = alias;
    inst.base_id = id;
    inst.position = position;
    inst.base_volume = volume;
    active_.push_back(inst);
  }
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
  std::erase_if(active_, [](const ActiveInstance &a) {
    if (!IsSoundPlaying(a.sound)) {
      UnloadSoundAlias(a.sound);
      return true;
    }
    return false;
  });

  for (const auto &a : active_) {
    const Vector2 src_pos = a.position;

    const Vector2 delta = Vector2Subtract(src_pos, listener_position);
    const float distance = Vector2Length(delta);

    constexpr float pan_range = 300.0F;
    const float pan_x = std::clamp(delta.x / pan_range, -1.0F, 1.0F);
    const float pan_y = std::clamp(delta.y / pan_range, -1.0F, 1.0F);

    float pan = 0.5F - ((pan_x * 0.5F) * (1.0F - (0.2F * std::fabs(pan_y))));
    pan = std::clamp(pan, 0.0F, 1.0F);

    constexpr float ref_distance = 32.0F;
    float volume = a.base_volume * (ref_distance / (ref_distance + distance));
    volume = std::clamp(volume, 0.0F, 1.0F);

    SetSoundPan(a.sound, pan);
    SetSoundVolume(a.sound, volume);
  }
}
}// namespace chroma::client::audio
