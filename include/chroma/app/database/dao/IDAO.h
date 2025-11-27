#pragma once

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/settings/SettingsManager.h"

#include <optional>

namespace chroma::app::database {
class ISettingsDAO
{
public:
  virtual ~ISettingsDAO() = default;

  virtual std::optional<settings::GameConfig> Load() = 0;
  virtual void Save(const settings::GameConfig &config) = 0;
};

class IPlayerDAO
{
public:
  virtual ~IPlayerDAO() = default;

  virtual std::optional<PlayerData> Load() = 0;
  virtual void Save(const PlayerData &data) = 0;
};
}// namespace chroma::app::database