#pragma once

#include "chroma/shared/events/AudioVolumeEvent.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

#include <string>
#include <vector>

namespace chroma::app::settings {

struct LevelConfig
{
  std::string id;
  std::string display_name;
  std::string system_path;
  std::string render_path;
};

struct GameConfig
{
  int id = 1;
  bool fullscreen = false;
  float master_volume = 1.0F;
  float music_volume = 1.0F;
  float sfx_volume = 1.0F;
  std::string selected_level_id = "plains";
};

class SettingsManager
{
public:
  static SettingsManager &Instance()
  {
    static SettingsManager instance;
    return instance;
  }

  SettingsManager(const SettingsManager &) = delete;
  void operator=(const SettingsManager &) = delete;

  void InitEventListener();
  void OnEvent(const shared::event::Event &event);
  void OnAudioVolumeEvent(const shared::event::AudioVolumeEvent &event);

  void SetGameConfig(GameConfig game_config);
  const GameConfig &GetGameConfig() const { return game_config_; }
  void ApplyCurrentSettings() const;

  // Level selection
  void SetSelectedLevel(const std::string &level_id);
  [[nodiscard]] const std::string &GetSelectedLevelId() const { return game_config_.selected_level_id; }
  [[nodiscard]] const LevelConfig &GetSelectedLevel() const;
  [[nodiscard]] const std::vector<LevelConfig> &GetAvailableLevels() const { return available_levels_; }

private:
  SettingsManager();

  void InitializeLevels();

  GameConfig game_config_;
  std::vector<LevelConfig> available_levels_;
  shared::event::Subscription audio_volume_sub_;
  shared::event::Subscription level_select_sub_;
};
}// namespace chroma::app::settings