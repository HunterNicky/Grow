#include "chroma/app/database/dao/SettingsDAO.h"

#include <exception>
#include <iostream>
#include <mutex>
#include <sqlite_orm/sqlite_orm.h>
#include <thread>

namespace chroma::app::database {

using namespace sqlite_orm;

SettingsDAO::SettingsDAO(StorageType &storage) : storage_(storage) {}

std::optional<settings::GameConfig> SettingsDAO::Load()
{
  auto rows = storage_.get_all<settings::GameConfig>(where(c(&settings::GameConfig::id) == 1));

  if (rows.empty()) {
    settings::GameConfig defaults;
    defaults.id = 1;
    storage_.insert(defaults);

    try {
      const std::scoped_lock<std::mutex> lock(save_mutex_);
      storage_.insert(defaults);
    } catch (...) {
      std::cerr << "[SettingsDAO] Falha ao criar configurações padrão.\n";
      return std::nullopt;
    }

    return defaults;
  }
  return rows.front();
}

void SettingsDAO::Save(const settings::GameConfig &config)
{
  const auto &temp = config;
  std::thread([this, temp]() noexcept {// NOLINT(bugprone-exception-escape)
    try {
      const std::scoped_lock<std::mutex> lock(save_mutex_);
      storage_.replace(temp);
    } catch (const std::exception &e) {
      std::cerr << "[SettingsRepository] Erro ao salvar: " << e.what() << '\n';
    } catch (...) {
      std::cerr << "[SettingsRepository] Erro desconhecido ao salvar configurações." << '\n';
    }
  })
    .detach();
}

}// namespace chroma::app::database