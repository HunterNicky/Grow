#include "chroma/app/settings/PlayerDataManager.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/PlayerDataEvent.h"

#include <memory>

namespace chroma::app::settings {

void PlayerDataManager::SetPlayerData(const database::PlayerData &player_data) { current_data_ = player_data; }

database::PlayerData &PlayerDataManager::GetPlayerData() { return current_data_; }

void PlayerDataManager::RequestSave()
{
  shared::event::PlayerDataEvent event(shared::event::PlayerDataAction::Save, current_data_);
  shared::event::EventBus::GetDispatcher()->Dispatch(event);
}

}// namespace chroma::app::settings