#include "chroma/shared/core/world/World.h"

#include "chroma/shared/collision/CollisionEvent.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/world/WorldNavigation.h"
#include "chroma/shared/core/components/world/WorldRender.h"
#include "chroma/shared/core/components/world/WorldSystem.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <print>
#include <raylib.h>

namespace chroma::shared::core::world {
World::World() { type_ = GameObjectType::WORLD; }

World::~World() = default;

void World::OnRender()
{
  static bool render_enabled = true;
  if (IsKeyPressed(KEY_T)) {
    render_enabled = !render_enabled;
    std::print("World rendering toggled {}\n", render_enabled ? "ON" : "OFF");
  }

  if (!render_enabled) { return; }

  const auto world_system = GetComponent<component::WorldSystem>();
  if (world_system) { world_system->Render(); }
  const auto world_render = GetComponent<component::WorldRender>();
  if (world_render) { world_render->Render(); }
  const auto world_navigation = GetComponent<component::WorldNavigation>();
  if (world_navigation) { world_navigation->Render(); }
}

void World::OnUpdate([[maybe_unused]] float delta_time) {}

std::shared_ptr<GameObject> World::Clone() { return std::make_shared<World>(*this); }

void World::OnFixedUpdate([[maybe_unused]] float fixed_delta_time) {}

void World::OnCollision([[maybe_unused]] const collision::CollisionEvent &collision) {}

void World::HandleEvent([[maybe_unused]] event::Event &event) {}
}// namespace chroma::shared::core::world