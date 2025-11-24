#pragma once

#include "GameContext.h"

#include <memory>
#include <unordered_map>

namespace chroma::shared::context {
class GameContextManager
{
public:
  static GameContextManager &Instance();

  std::shared_ptr<GameContext> GetContext(GameContextType type);
  void SetContext(GameContextType type, const std::shared_ptr<GameContext> &context);
  void CreateContext(GameContextType type);
  void DeleteContext(GameContextType type);

  void SetDeltaTime(float delta_time);

private:
  GameContextManager() = default;
  ~GameContextManager() = default;

  std::unordered_map<GameContextType, std::shared_ptr<GameContext>> contexts_;
};
}// namespace chroma::shared::context

using GCM = chroma::shared::context::GameContextManager;
