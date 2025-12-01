#include "chroma/app/database/repositories/SettingsRepository.h"
#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/settings/SettingsManager.h"

#include <mutex>
#include <sqlite_orm/sqlite_orm.h>
#include <thread>
#include <iostream>
#include <exception>

namespace chroma::app::database {
SettingsRepository::SettingsRepository(StorageType &storage) : storage_(storage) {}

settings::GameConfig SettingsRepository::Load()
{
  auto rows = storage_.get_all<settings::GameConfig>(where(c(&settings::GameConfig::id) == 1));

  if (rows.empty()) {
    settings::GameConfig defaults;
    defaults.id = 1;
    storage_.insert(defaults);
    return defaults;
  }
  return rows.front();
}

void SettingsRepository::Save()
{
  const auto &temp = chroma::app::settings::SettingsManager::Instance().GetGameConfig();
  std::thread([this, temp]() noexcept { // NOLINT(bugprone-exception-escape)
    try {
      const std::scoped_lock<std::mutex> lock(save_mutex_);
      storage_.replace(temp);
    } catch (const std::exception &e) {
      std::cerr << "[SettingsRepository] Erro ao salvar: " << e.what() << '\n';
    }
    catch (...) {
      std::cerr << "[SettingsRepository] Erro desconhecido ao salvar configurações." << '\n';
    }
  }).detach();
}
}// namespace chroma::app::database