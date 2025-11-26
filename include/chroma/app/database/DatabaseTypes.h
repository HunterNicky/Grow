#pragma once

#include "chroma/app/settings/SettingsManager.h"

#include <sqlite_orm/sqlite_orm.h>
#include <string>

namespace chroma::app::database {

using namespace sqlite_orm;

struct PlayerData
{
  int id = 1;
  int character_id = 1;
  int level = 1;
  int current_xp = 0;
  int hp = 120;
  int pos_x = 0;
  int pos_y = 0;
  int coins = 0;
  int weapon_id = 0;
  int direction = 0;
  bool is_left = false;
  int character_skin = 0;
};

inline auto InitStorage(const std::string &path)
{
  return make_storage(path,
    make_table("settings",
      make_column("id", &settings::GameConfig::id, primary_key()),
      make_column("fullscreen", &settings::GameConfig::fullscreen),
      make_column("master_volume", &settings::GameConfig::master_volume),
      make_column("music_volume", &settings::GameConfig::music_volume),
      make_column("sfx_volume", &settings::GameConfig::sfx_volume)),
      
    make_table("player_data",
      make_column("id", &PlayerData::id, primary_key()),
      make_column("character_id", &PlayerData::character_id),
      make_column("level", &PlayerData::level),
      make_column("current_xp", &PlayerData::current_xp),
      make_column("hp", &PlayerData::hp),
      make_column("pos_x", &PlayerData::pos_x),
      make_column("pos_y", &PlayerData::pos_y),
      make_column("coins", &PlayerData::coins),
      make_column("weapon_id", &PlayerData::weapon_id),
      make_column("direction", &PlayerData::direction),
      make_column("is_left", &PlayerData::is_left),
      make_column("character_skin", &PlayerData::character_skin)));
      
}

using StorageType = decltype(InitStorage(""));
}// namespace chroma::app::database