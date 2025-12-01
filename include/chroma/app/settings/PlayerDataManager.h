#pragma once

#include "chroma/app/database/DatabaseTypes.h"

namespace chroma::app::settings {

class PlayerDataManager
{
public:
  static PlayerDataManager& Instance()
  {
    static PlayerDataManager instance;
    return instance;
  }
  
  PlayerDataManager(const PlayerDataManager&) = delete;
  PlayerDataManager& operator=(const PlayerDataManager&) = delete;
  PlayerDataManager(PlayerDataManager&&) = delete;
  PlayerDataManager& operator=(PlayerDataManager&&) = delete;

  void SetPlayerData(const database::PlayerData &player_data);
  database::PlayerData &GetPlayerData();
  void RequestSave() const;

private:
  PlayerDataManager() = default;
  ~PlayerDataManager() = default;

  database::PlayerData current_data_;
};

} // namespace chroma::app::settings