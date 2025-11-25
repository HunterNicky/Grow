#pragma once

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/settings/SettingsManager.h"

#include <sqlite_orm/sqlite_orm.h>
#include <mutex>

namespace chroma::app::database {
class SettingsRepository
{
public:
  explicit SettingsRepository(StorageType &storage);

  settings::GameConfig Load();
  void Save();

private:
  StorageType &storage_;
  std::mutex save_mutex_;
};
}// namespace chroma::client::database