#include "chroma/shared/factory/GameObjectFactory.h"

#include "chroma/shared/core/components//SpriteAnimation.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/GameObject.h"


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

          auto player = builder::GameObjectBuilder<core::player::Player>()
                          .Id(entity_id)
                          .AddAnimation()
                          .AddCamera(render::CameraMode::FollowSmooth, 3.0F, 2.0F, { 64, 128 })
                          .AddAudioListener()
                          .NetRole(is_local_player ? core::NetRole::AUTONOMOUS : core::NetRole::SIMULATED)
                          .Build();

          player->SetupAnimation(player->GetComponent<core::component::SpriteAnimation>());
          return player;
        });
    }
    if (!reg.contains(Game::GameObjectType::Projectile)) {
      reg.emplace(Game::GameObjectType::Projectile,
        [](const Game::EntityState *entity_state, const bool /*is_local_player*/) -> std::shared_ptr<core::GameObject> {
          if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
          const UUIDv4::UUID entity_id(entity_state->id()->str());

          auto projectile = builder::GameObjectBuilder<core::projectile::Projectile>()
                              .Id(entity_id)
                              .AddAnimation()
                              .NetRole(core::NetRole::SIMULATED)
                              .Build();

          return projectile;
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
