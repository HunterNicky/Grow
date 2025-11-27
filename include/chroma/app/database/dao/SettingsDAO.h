#pragma once

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/database/dao/IDAO.h"

#include <optional>
#include <mutex>

namespace chroma::app::database {
class SettingsDAO : public ISettingsDAO
{
public:
  explicit SettingsDAO(StorageType &storage);
  std::optional<settings::GameConfig> Load() override;
  void Save(const settings::GameConfig& config) override;

private:
  StorageType& storage_;
  std::mutex save_mutex_;
};
}// namespace chroma::app::database