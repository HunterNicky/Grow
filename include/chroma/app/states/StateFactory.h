#pragma once

#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace chroma::app::states {
class StateFactory
{
public:
  StateFactory() = default;

  void Register(const StateID state_id, std::function<std::shared_ptr<State>()> recipe);
  std::shared_ptr<State> Create(const StateID state_id);

private:
  std::unordered_map<StateID, std::function<std::shared_ptr<State>()>> factory_;
};
}// namespace chroma::app::states