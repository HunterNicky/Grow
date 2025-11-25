#include "chroma/shared/core/components/world/WorldSystem.h"

#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::core::component {
WorldSystem::WorldSystem() : world_system_(std::make_unique<world::WorldSystem>())
{
  type_ = ComponentType::WORLD_SYSTEM;
}

void WorldSystem::Initialize(const std::string &path)
{
  world_system_->Initialize(path);
  const auto colliders = world_system_->GetAllColliders();
  const auto game_object_id = GetGameObject()->GetId();
  for (const auto &col : colliders) {
    collision::ColliderEntry const entry{
      .id = game_object_id,
      .bounds = std::make_shared<Rectangle>(col.rect),
      .component = std::static_pointer_cast<Component>(shared_from_this()),
    };

    if (HasAuthority()) {
      GCM::Instance()
        .GetContext(GameContextType::Server)
        ->GetCollisionManager()
        ->AddCollider(entry, collision::BodyType::Static);
    } else {
      GCM::Instance()
        .GetContext(GameContextType::Client)
        ->GetCollisionManager()
        ->AddCollider(entry, collision::BodyType::Static);
    }
  }
}
std::vector<world::RenderTile> WorldSystem::GetRenderTile() const
{
  if (world_system_) { return world_system_->GetRenderTile(); }
  return {};
}

void WorldSystem::Update([[maybe_unused]] float delta_time) {}

void WorldSystem::Render() { world_system_->RenderDebugColliders(); }
}// namespace chroma::shared::core::component