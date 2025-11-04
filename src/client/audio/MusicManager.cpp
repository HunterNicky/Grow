#include "chroma/client/audio/MusicManager.h"

#include <optional>
#include <raylib.h>
#include <string>
#include <vector>

namespace chroma::client::audio {

MusicManager::MusicManager() = default;

MusicManager::~MusicManager()
{
  for (const auto &pair : music_) { UnloadMusicStream(pair.second); }
  music_.clear();
}

void MusicManager::LoadMusic(const std::string &id, const std::string &file_path)
{
  if (music_.contains(id)) { return; }
  const Music music = LoadMusicStream(file_path.c_str());
  music_[id] = music;
}

void MusicManager::PlayMusic(const std::string &id, const bool loop, const float volume)
{
  const auto it = music_.find(id);
  if (it != music_.end()) {
    StopMusic();
    SetMusicVolume(it->second, volume);
    PlayMusicStream(it->second);
    if (loop) { it->second.looping = true; }
    current_id_.clear();
    current_music_id_ = id;
    current_volume_ = volume;
  }
}

void MusicManager::StopMusic()
{
  if (!current_music_id_.empty()) {
    const auto it = music_.find(current_music_id_);
    if (it != music_.end()) { StopMusicStream(it->second); }
  }
  current_music_id_.clear();
  current_id_.clear();
}

void MusicManager::Update()
{
  if (current_music_id_.empty()) { return; }

  {
    const auto it_music = music_.find(current_music_id_);
    if (it_music != music_.end()) { UpdateMusicStream(it_music->second); }
  }

  const auto it_playlist = music_queue_.find(current_id_);
  if (it_playlist == music_queue_.end()) { return; }

  auto &queue = it_playlist->second;
  if (!queue.empty()) {
    const std::string &current_track_id = queue.front();
    const auto it_current = music_.find(current_track_id);
    if (it_current == music_.end()) {
      queue.pop();
    } else {
      if (current_music_id_ != current_track_id) {
        it_current->second.looping = false;
        SetMusicVolume(it_current->second, current_volume_);
        PlayMusicStream(it_current->second);
        current_music_id_ = current_track_id;
      }

      if (!IsMusicStreamPlaying(it_current->second)) {
        StopMusicStream(it_current->second);
        queue.pop();
        if (!queue.empty()) {
          const std::string &next_id = queue.front();
          const auto it_next = music_.find(next_id);
          if (it_next != music_.end()) {
            it_next->second.looping = false;
            SetMusicVolume(it_next->second, current_volume_);
            PlayMusicStream(it_next->second);
            current_music_id_ = next_id;
          }
        } else {
          current_id_.clear();
          current_music_id_.clear();
        }
      }
    }
  }
}

void MusicManager::UnloadMusic(const std::string &id)
{
  const auto it = music_.find(id);
  if (it != music_.end()) {
    UnloadMusicStream(it->second);
    music_.erase(it);
    if (current_music_id_ == id) { current_music_id_.clear(); }
  }
}

void MusicManager::AddMusicPlaylist(const std::string &playlist_id, const std::string &music_id)
{
  if (music_.contains(music_id)) { music_queue_[playlist_id].push(music_id); }
}

void MusicManager::AddMusicPlaylist(const std::string &playlist_id, const std::vector<std::string> &music_ids)
{
  for (const std::string &id : music_ids) {
    if (music_.contains(id)) { music_queue_[playlist_id].push(id); }
  }
}

void MusicManager::PlayPlaylist(const std::string &playlist_id, const float volume)
{
  const auto it = music_queue_.find(playlist_id);
  if (it == music_queue_.end() || it->second.empty()) { return; }

  StopMusic();

  current_volume_ = volume;

  const std::string &music_id = it->second.front();
  const auto it_music = music_.find(music_id);
  if (it_music != music_.end()) {
    it_music->second.looping = false;
    SetMusicVolume(it_music->second, volume);
    PlayMusicStream(it_music->second);
    current_music_id_ = music_id;
    current_id_ = playlist_id;
  }
}

void MusicManager::CleanPlaylist(const std::string &playlist_id) { music_queue_.erase(playlist_id); }

std::optional<std::string> MusicManager::GetCurrentMusic() const
{
  if (current_music_id_.empty()) { return std::nullopt; }
  return current_music_id_;
}

}// namespace chroma::client::audio
