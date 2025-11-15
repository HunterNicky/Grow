#include "chroma/shared/factory/GameObjectFactory.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/builder/GameObjectBuilder.h"

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
           
          auto player = chroma::shared::builder::GameObjectBuilder<chroma::shared::core::player::Player>()
            .AddTransform({0,0})
            .Id(entity_id)
            .AddSpeed(50.0F)
            .AddMovement()
            .AddAnimation()
            .AddCamera(chroma::shared::render::CameraMode::FollowSmooth, 3.0F, 2.0F, {64,128})
            .AddAudioListener()
            .AddHealth(100.0F, 1.0F)
            .NetRole(is_local_player ? core::NetRole::ROLE_AutonomousProxy : core::NetRole::ROLE_SimulatedProxy)
            .Build();

          player->SetupAnimation(player->GetComponent<shared::core::component::SpriteAnimation>());
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
