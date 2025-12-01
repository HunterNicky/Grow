#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/context/GameContext.h"

#include <memory>
#include <ranges>

namespace chroma::shared::context {

GameContextManager &GameContextManager::Instance()
{
  static GameContextManager instance;
  return instance;
}

std::shared_ptr<GameContext> GameContextManager::GetContext(const GameContextType type)
{
  if (contexts_.contains(type)) { return contexts_[type]; }
  CreateContext(type);
  return contexts_[type];
}

void GameContextManager::SetContext(const GameContextType type, const std::shared_ptr<GameContext> &context)
{
  if (context) { contexts_[type] = context; }
}

void GameContextManager::CreateContext(const GameContextType type)
{
  const auto context = std::make_shared<GameContext>();
  context->SetType(type);
  contexts_[type] = context;
}

void GameContextManager::DeleteContext(const GameContextType type)
{
  if (contexts_.contains(type)) { contexts_.erase(type); }
}

void GameContextManager::SetDeltaTime(const float delta_time) const
{
  for (const auto &context : contexts_ | std::views::values) { context->SetDeltaTime(delta_time); }
}
}// namespace chroma::shared::context