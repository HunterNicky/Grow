#include "chroma/app/database/DatabaseManager.h"
#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/database/dao/PlayerDAO.h"
#include "chroma/app/database/dao/SettingsDAO.h"
#include "chroma/app/settings/PlayerDataManager.h"
#include "chroma/app/settings/SettingsManager.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/PlayerDataEvent.h"
#include "chroma/shared/events/SaveSettingsEvent.h"

#include <memory>
#include <string>

namespace chroma::app::database {
DatabaseManager::DatabaseManager(const std::string &db_path)
  : storage_(InitStorage(db_path)), settings_dao_(std::make_unique<SettingsDAO>(storage_)),
    player_dao_(std::make_unique<PlayerDAO>(storage_))
{
  storage_.sync_schema();
}

void DatabaseManager::InitEventListener()
{
  settings_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::SaveSettingsEvent>(
    [this](const shared::event::Event &event) { this->OnEvent(event); });

  player_data_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::PlayerDataEvent>(
    [this](const shared::event::Event &event) { this->OnEvent(event); });
}

void DatabaseManager::OnEvent(const shared::event::Event &event)
{
  switch (event.GetType()) {
  case shared::event::Event::Type::SaveSettingsEvent: {
    settings_dao_->Save(settings::SettingsManager::Instance().GetGameConfig());
    break;
  }
  case shared::event::Event::Type::PlayerDataEvent: {
    const auto &player_event = static_cast<const shared::event::PlayerDataEvent &>(event);

    if (player_event.GetAction() == shared::event::PlayerDataAction::Save) {
      player_dao_->Save(player_event.GetData());
    } else if (player_event.GetAction() == shared::event::PlayerDataAction::Load) {
      auto data = player_dao_->Load();

      if (data.has_value()) { settings::PlayerDataManager::Instance().SetPlayerData(data.value()); }
    }
    break;
  }
  }
}

StorageType &DatabaseManager::GetStorage() { return storage_; }

ISettingsDAO &DatabaseManager::GetSettingsDAO() { return *settings_dao_; }

IPlayerDAO &DatabaseManager::GetPlayerDAO() { return *player_dao_; }

}// namespace chroma::app::database