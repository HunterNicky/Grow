#include "chroma/app/database/dao/PlayerDAO.h"
#include "chroma/app/database/DatabaseTypes.h"

#include <exception>
#include <iostream>
#include <mutex>
#include <optional>
#include <sqlite_orm/sqlite_orm.h>
#include <thread>

namespace chroma::app::database {

PlayerDAO::PlayerDAO(StorageType &storage) : storage_(storage) {}

std::optional<PlayerData> PlayerDAO::Load()
{
  std::scoped_lock<std::mutex> const lock(save_mutex_);

  auto result = storage_.get_all<PlayerData>(where(c(&PlayerData::id) == 1));

  if (result.empty()) { return std::nullopt; }

  return result.front();
}

void PlayerDAO::Save(const PlayerData &data)
{
  // NOLINTNEXTLINE
  std::thread([this, data]() noexcept {
    try {
      const std::scoped_lock<std::mutex> lock(save_mutex_);
      storage_.replace(data);

    } catch (const std::exception &e) {
      std::cerr << "[PlayerDAO] Erro ao salvar assincronamente: " << e.what() << '\n';
    } catch (...) {
      std::cerr << "[PlayerDAO] Erro desconhecido ao salvar." << '\n';
    }
  }).detach();
}

}// namespace chroma::app::database