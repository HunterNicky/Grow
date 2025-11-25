#pragma once

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/database/repositories/SettingsRepository.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/Event.h"

#include <sqlite_orm/sqlite_orm.h>
#include <string>

namespace chroma::app::database {
class DatabaseManager
{
public:
  explicit DatabaseManager(const std::string &db_path);
  StorageType &GetStorage();
  SettingsRepository &GetSettingsRepo();
  void InitEventListener();

  void OnEvent(const shared::event::Event &event);

private:
  StorageType storage_;
  SettingsRepository settings_repo_;

  shared::event::Subscription settings_sub_;
};
}// namespace chroma::app::database