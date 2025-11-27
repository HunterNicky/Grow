#include "chroma/app/states/StateFactory.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>

namespace chroma::app::states {
void StateFactory::Register(const StateID state_id, std::function<std::shared_ptr<State>()> recipe)
{
  factory_[state_id] = std::move(recipe);
}

std::shared_ptr<State> StateFactory::Create(const StateID state_id)
{
  auto it = factory_.find(state_id);
  if (it != factory_.end()) { return it->second(); }

  return nullptr;
}
}// namespace chroma::app::states