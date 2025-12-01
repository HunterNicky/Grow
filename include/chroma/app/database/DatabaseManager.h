#pragma once

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/Event.h"
#include "chroma/app/database/dao/IDAO.h"

#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <memory>

namespace chroma::app::database {
class DatabaseManager
{
public:
  explicit DatabaseManager(const std::string &db_path);
  StorageType &GetStorage();
  ISettingsDAO& GetSettingsDAO() const;
  IPlayerDAO& GetPlayerDAO() const;
  void InitEventListener();

  void OnEvent(const shared::event::Event &event) const;

private:
  StorageType storage_;
  std::unique_ptr<ISettingsDAO> settings_dao_;
  std::unique_ptr<IPlayerDAO> player_dao_;

  shared::event::Subscription settings_sub_;
  shared::event::Subscription player_data_sub_;
};
}// namespace chroma::app::database