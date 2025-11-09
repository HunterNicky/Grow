#include "chroma/shared/factory/GameObjectFactory.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "entities_generated.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <uuid_v4.h>

namespace chroma::shared::factory {
namespace {
  std::unordered_map<Game::GameObjectType, GameObjectFactory::Creator> &Registry()
  {
    static std::unordered_map<Game::GameObjectType, GameObjectFactory::Creator> reg;
    return reg;
  }

  void EnsureDefaults()
  {
    auto &reg = Registry();
    if (!reg.contains(Game::GameObjectType::Player)) {
      reg.emplace(Game::GameObjectType::Player,
        [](const Game::EntityState *entity_state, const bool is_local_player) -> std::shared_ptr<core::GameObject> {
          if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
          const UUIDv4::UUID entity_id(entity_state->id()->str());
          auto player = std::make_shared<core::player::Player>();
          player->SetNetRole(
            is_local_player ? core::NetRole::ROLE_AutonomousProxy : core::NetRole::ROLE_SimulatedProxy);
          player->InitComponents();
          player->SetId(entity_id);
          return player;
        });
    }
  }
}// namespace

std::shared_ptr<core::GameObject> GameObjectFactory::Create(const Game::EntityState *entity_state,
  const bool is_local_player)
{
  if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
  EnsureDefaults();
  const auto &reg = Registry();
  const auto it = reg.find(entity_state->type());
  if (it == reg.end()) { return nullptr; }
  return it->second(entity_state, is_local_player);
}

void GameObjectFactory::Register(const Game::GameObjectType type, Creator creator)
{
  EnsureDefaults();
  Registry()[type] = std::move(creator);
}

}// namespace chroma::shared::factory
