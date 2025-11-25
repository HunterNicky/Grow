#pragma once

#include <optional>
#include <queue>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace chroma::client::audio {

class MusicManager
{
public:
  MusicManager();
  ~MusicManager();

  void LoadMusic(const std::string &id, const std::string &file_path);
  void PlayMusic(const std::string &id, bool loop = true, float volume = 1.0f);
  void StopMusic();
  void Update();
  void UnloadMusic(const std::string &id);

  void AddMusicPlaylist(const std::string &playlist_id, const std::string &music_id);
  void AddMusicPlaylist(const std::string &playlist_id, const std::vector<std::string> &music_ids);
  void PlayPlaylist(const std::string &playlist_id, float volume = 1.0f);
  void CleanPlaylist(const std::string &playlist_id);

  [[nodiscard]] std::optional<std::string> GetCurrentMusic() const;

private:
  std::unordered_map<std::string, std::queue<std::string>> music_queue_;
  std::unordered_map<std::string, Music> music_;
  std::string current_id_;
  std::string current_music_id_;

  float current_volume_ = 0.0F;
};

}// namespace chroma::client::audio
