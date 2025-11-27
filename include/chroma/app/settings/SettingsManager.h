#pragma once

#include "chroma/shared/events/AudioVolumeEvent.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app::settings {

struct GameConfig
{
  int id = 1;
  bool fullscreen = false;
  float master_volume = 1.0F;
  float music_volume = 1.0F;
  float sfx_volume = 1.0F;
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

private:
  SettingsManager() = default;

  GameConfig game_config_;
  shared::event::Subscription audio_volume_sub_;
};
}// namespace chroma::app::settings