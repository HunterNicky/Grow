#include "chroma/shared/factory/GameObjectFactory.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/enemy/Enemy.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/core/world/World.h"
#include "chroma/shared/packet/adapter/ComponentAdapter.h"
#include "chroma/shared/render/RenderBridge.h"
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

  void Player(std::unordered_map<Game::GameObjectType, GameObjectFactory::Creator> &reg)
  {
    reg.emplace(Game::GameObjectType::Player,
      [](const Game::EntityState *entity_state, const bool is_local_player) -> std::shared_ptr<core::GameObject> {
        if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
        const UUIDv4::UUID entity_id = UUIDv4::UUID::fromStrFactory(entity_state->id()->str());

        builder::GameObjectBuilder<core::player::Player> player_build =
          builder::GameObjectBuilder<core::player::Player>()
            .Id(entity_id)
            .AddTransform(entity_state, { 1.0F, 1.0F })
            .AddSpeed(50.0F)
            .AddMovement()
            .AddAnimation()
            .AddCamera(render::CameraMode::FollowSmooth, 3.0F, 2.0F, { 64, 128 })
            .AddColliderBox(GameContextType::Client, { 12.F, 12.F }, { -6.F, 0.F })
            .AddEventColliderBox(GameContextType::Client, { 32.F, 32.F }, { -16.F, -16.F })
            .AddHealth(100.0F, 100.0F)
            .AddRun(false, 1.5F)
            .AddInventory(10)
            .AddCharacterType(packet::adapter::ComponentAdapter::GetCharacterTypeFromEntityState(entity_state));

        if (is_local_player) {
          player_build.AddAudioListener()
            .AddCamera(render::CameraMode::FollowSmooth, 3.0F, 2.0F, { 64, 128 })
            .NetRole(core::NetRole::AUTONOMOUS);
        } else {
          player_build.NetRole(core::NetRole::SIMULATED);
        }

        auto player = player_build.Build();

        player->SetupAnimation(player->GetComponent<core::component::SpriteAnimation>());
        return player;
      });
  }

  void Projectile(std::unordered_map<Game::GameObjectType, GameObjectFactory::Creator> &reg)
  {
    reg.emplace(Game::GameObjectType::Projectile,
      [](const Game::EntityState *entity_state,
        [[maybe_unused]] const bool is_local_player) -> std::shared_ptr<core::GameObject> {
        if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
        const UUIDv4::UUID entity_id = UUIDv4::UUID::fromStrFactory(entity_state->id()->str());

        auto projectile = builder::GameObjectBuilder<core::projectile::Projectile>()
                            .Id(entity_id)
                            .AddAnimation()
                            .NetRole(core::NetRole::SIMULATED)
                            .Build();

        return projectile;
      });
  }

  void World(std::unordered_map<Game::GameObjectType, GameObjectFactory::Creator> &reg)
  {
    reg.emplace(Game::GameObjectType::World,
      [](const Game::EntityState *entity_state,
        [[maybe_unused]] const bool is_local_player) -> std::shared_ptr<core::GameObject> {
        if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
        const UUIDv4::UUID entity_id = UUIDv4::UUID::fromStrFactory(entity_state->id()->str());

        auto world = builder::GameObjectBuilder<core::world::World>()
                       .Id(entity_id)
                       .AddWorldSystem("assets/world/plains.json")
                       .AddWorldRender("assets/sprites/world/plains-world.json")
                       .AddWorldNavigation()
                       .NetRole(core::NetRole::SIMULATED)
                       .Build();

        return world;
      });
  }

  void Enemy(std::unordered_map<Game::GameObjectType, GameObjectFactory::Creator> &reg)
  {
    reg.emplace(Game::GameObjectType::Enemy,
      [](const Game::EntityState *entity_state,
        [[maybe_unused]] const bool is_local_player) -> std::shared_ptr<core::GameObject> {
        if (entity_state == nullptr || entity_state->id() == nullptr) { return nullptr; }
        const UUIDv4::UUID entity_id = UUIDv4::UUID::fromStrFactory(entity_state->id()->str());

        auto enemy = builder::GameObjectBuilder<core::enemy::Enemy>()
                       .Id(entity_id)
                       .AddTransform(entity_state, { 1.0F, 1.0F })
                       .AddSpeed(30.0F)
                       .AddHealth(50.0F, 40.0F)
                       .AddMovement()
                       .AddAnimation()
                       .AddColliderBox(GameContextType::Client, { 12.F, 12.F }, { -6.F, 0.F })
                       .AddEventColliderBox(GameContextType::Client, { 32.F, 32.F }, { -16.F, -16.F })
                       .NetRole(core::NetRole::SIMULATED)
                       .Build();

        enemy->SetupAnimation();

        return enemy;
      });
  }

  void EnsureDefaults()
  {
    auto &reg = Registry();
    if (!reg.contains(Game::GameObjectType::Player)) { Player(reg); }
    if (!reg.contains(Game::GameObjectType::Projectile)) { Projectile(reg); }
    if (!reg.contains(Game::GameObjectType::World)) { World(reg); }
    if (!reg.contains(Game::GameObjectType::Enemy)) { Enemy(reg); }
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

  auto obj = it->second(entity_state, is_local_player);

  return obj;
}

void GameObjectFactory::Register(const Game::GameObjectType type, Creator creator)
{
  EnsureDefaults();
  Registry()[type] = std::move(creator);
}

}// namespace chroma::shared::factory
