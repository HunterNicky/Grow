#include "chroma/app/settings/SettingsManager.h"
#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/events/AudioVolumeEvent.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/Subscription.h"

#include <algorithm>
#include <string>

namespace chroma::app::settings {

SettingsManager::SettingsManager()
{
  InitializeLevels();
}

void SettingsManager::InitializeLevels()
{
  available_levels_ = {
    { .id = "plains",
      .display_name = "Plains",
      .system_path = "assets/world/plains.json",
      .render_path = "assets/sprites/world/plains-world.json" },
    { .id = "caves",
      .display_name = "Caves",
      .system_path = "assets/world/caves.json",
      .render_path = "assets/sprites/world/caves-world.json" },
    { .id = "desert",
      .display_name = "Desert",
      .system_path = "assets/world/desert.json",
      .render_path = "assets/sprites/world/desert-world.json" },
  };
}

void SettingsManager::InitEventListener()
{
  audio_volume_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::AudioVolumeEvent>(
    [this](const shared::event::Event &event) { this->OnEvent(event); });
}

void SettingsManager::OnEvent(const shared::event::Event &event)
{
  switch (event.GetType()) {
  case shared::event::Event::Type::AudioVolumeEvent: {
    const auto &audio_event = dynamic_cast<const shared::event::AudioVolumeEvent &>(event);
    OnAudioVolumeEvent(audio_event);
    break;
  }
  default:
    break;
  }
}

void SettingsManager::OnAudioVolumeEvent(const shared::event::AudioVolumeEvent &event)
{
  switch (event.GetChannel()) {
  case shared::event::AudioChannel::Master:
    game_config_.master_volume = event.GetVolume();
    shared::audio::GetAudioBridge()->SetMasterVolume(game_config_.master_volume);
    break;
  case shared::event::AudioChannel::Music:
    game_config_.music_volume = event.GetVolume();
    break;
  case shared::event::AudioChannel::SFX:
    game_config_.sfx_volume = event.GetVolume();
    break;
  }
}

void SettingsManager::SetGameConfig(GameConfig game_config) { game_config_ = game_config; }

void SettingsManager::ApplyCurrentSettings() const
{
  shared::audio::GetAudioBridge()->SetMasterVolume(game_config_.master_volume);
}

void SettingsManager::SetSelectedLevel(const std::string &level_id)
{
  auto it = std::ranges::find_if(available_levels_, [&level_id](const LevelConfig &level) {
    return level.id == level_id;
  });
  
  if (it != available_levels_.end()) {
    game_config_.selected_level_id = level_id;
  }
}

const LevelConfig &SettingsManager::GetSelectedLevel() const
{
  auto it = std::ranges::find_if(available_levels_, [this](const LevelConfig &level) {
    return level.id == game_config_.selected_level_id;
  });
  
  if (it != available_levels_.end()) {
    return *it;
  }
  
  return available_levels_.front();
}

}// namespace chroma::app::settings