#include "chroma/app/settings/SettingsManager.h"
#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/events/AudioVolumeEvent.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app::settings {

void SettingsManager::InitEventListener()
{
  audio_volume_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::AudioVolumeEvent>(
    [this](const shared::event::Event &event) { this->OnEvent(event); });
}

void SettingsManager::OnEvent(const shared::event::Event &event)
{
  switch (event.GetType()) {
  case shared::event::Event::Type::AudioVolumeEvent: {
    const auto audio_event = dynamic_cast<const shared::event::AudioVolumeEvent &>(event);
    OnAudioVolumeEvent(audio_event);
    break;
  }
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

}// namespace chroma::app::settings