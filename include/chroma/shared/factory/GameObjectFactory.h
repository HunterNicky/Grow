#pragma once

#include <functional>
#include <memory>

#include "chroma/shared/core/GameObject.h"
#include "game_generated.h"

namespace chroma::shared::factory {

class GameObjectFactory
{
public:
  static std::shared_ptr<core::GameObject> Create(const Game::EntityState *entity_state, bool is_local_player);

  using Creator = std::function<std::shared_ptr<core::GameObject>(const Game::EntityState *, bool)>;
  static void Register(Game::GameObjectType type, Creator creator);
};

}// namespace chroma::shared::factory
