#pragma once

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/database/dao/IDAO.h"

#include <optional>
#include <mutex>

namespace chroma::app::database {
class PlayerDAO : public IPlayerDAO
{
public:
  explicit PlayerDAO(StorageType &storage);
  std::optional<PlayerData> Load() override;
  void Save(const PlayerData& data) override;

private:
  StorageType& storage_;
  std::mutex save_mutex_;
};
}// namespace chroma::app::database