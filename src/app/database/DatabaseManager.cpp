#include "chroma/app/database/DatabaseManager.h"
#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/database/repositories/SettingsRepository.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/SaveSettingsEvent.h"

#include <string>

namespace chroma::app::database {
DatabaseManager::DatabaseManager(const std::string &db_path) : storage_(InitStorage(db_path)), settings_repo_(storage_)
{
  storage_.sync_schema();
}

void DatabaseManager::InitEventListener()
{
  settings_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::SaveSettingsEvent>(
    [this](const shared::event::Event &event) { this->OnEvent(event); });
}

void DatabaseManager::OnEvent(const shared::event::Event &event)
{
  switch (event.GetType()) {
  case shared::event::Event::Type::SaveSettingsEvent: {
    settings_repo_.Save();
    break;
  }
  }
}

StorageType &DatabaseManager::GetStorage() { return storage_; }

SettingsRepository &DatabaseManager::GetSettingsRepo() { return settings_repo_; }

}// namespace chroma::app::database