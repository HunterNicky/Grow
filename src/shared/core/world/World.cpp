#include "chroma/shared/core/world/World.h"

#include "chroma/shared/core/components/world/WorldRender.h"
#include "chroma/shared/core/components/world/WorldSystem.h"

namespace chroma::shared::core::world {
World::World() { type_ = GameObjectType::WORLD; }

World::~World() = default;

void World::OnRender()
{
  const auto world_system = GetComponent<component::WorldSystem>();
  if (world_system) { world_system->Render(); }
  const auto world_render = GetComponent<component::WorldRender>();
  if (world_render) { world_render->Render(); }
}

void World::OnUpdate([[maybe_unused]] float delta_time) {}

std::shared_ptr<GameObject> World::Clone() { return std::make_shared<World>(*this); }

void World::OnFixedUpdate([[maybe_unused]] float fixed_delta_time) {}

void World::OnCollision([[maybe_unused]] const collision::CollisionEvent &collision) {}

void World::HandleEvent([[maybe_unused]] event::Event &event) {}
}// namespace chroma::shared::core::world