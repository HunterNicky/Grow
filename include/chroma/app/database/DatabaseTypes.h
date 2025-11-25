#pragma once

#include "chroma/app/settings/SettingsManager.h"

#include <sqlite_orm/sqlite_orm.h>
#include <string>

namespace chroma::app::database {

using namespace sqlite_orm;

inline auto InitStorage(const std::string &path)
{
  return make_storage(path,
    make_table("settings",
      make_column("id", &settings::GameConfig::id, primary_key()),
      make_column("fullscreen", &settings::GameConfig::fullscreen),
      make_column("master_volume", &settings::GameConfig::master_volume),
      make_column("music_volume", &settings::GameConfig::music_volume),
      make_column("sfx_volume", &settings::GameConfig::sfx_volume)));
}

using StorageType = decltype(InitStorage(""));
}// namespace chroma::app::database