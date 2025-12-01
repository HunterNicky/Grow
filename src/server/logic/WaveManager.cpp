#include "chroma/server/logic/WaveManager.h"

#include "chroma/shared/ai/Astar.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/world/WorldNavigation.h"
#include "chroma/shared/core/enemy/Enemy.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/WaveEvent.h"
#include "chroma/shared/utils/UUID.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>

namespace chroma::server::logic {
WaveManager::WaveManager() : current_wave_(0), max_waves_(10), spawn_interval_(1.0F), time_since_last_check_(0.0F) {}

void WaveManager::Update(const float delta_time)
{
  if (current_wave_ >= max_waves_) { return; }

  time_since_last_check_ += delta_time;
  if (time_since_last_check_ < spawn_interval_) { return; }
  time_since_last_check_ = 0.0F;

  if (current_wave_ == 0) {
    StartNextWave();
    return;
  }

  if (IsCurrentWaveCleared()) { StartNextWave(); }
}

uint32_t WaveManager::GetCurrentWave() const { return current_wave_; }

void WaveManager::StartNextWave()
{
  if (current_wave_ >= max_waves_) { return; }

  ++current_wave_;

  if (auto *dispatcher = shared::event::EventBus::GetDispatcher()) {
    shared::event::WaveEvent wave_event(current_wave_);
    dispatcher->Dispatch(wave_event);
  }

  const uint32_t enemy_count = current_wave_;
  SpawnEnemies(enemy_count);
}

bool WaveManager::IsCurrentWaveCleared()
{
  const auto context = shared::context::GameContextManager::Instance().GetContext(GameContextType::Server);
  if (!context) { return false; }

  const auto manager = context->GetGameObjectManager();
  if (!manager) { return false; }

  const auto &enemies = manager->GetByTag(shared::core::GameObjectType::ENEMY);
  return enemies.empty();
}

void WaveManager::SpawnEnemies(const uint32_t count)
{
  const auto context = shared::context::GameContextManager::Instance().GetContext(GameContextType::Server);
  if (!context) { return; }

  const auto manager = context->GetGameObjectManager();
  if (!manager) { return; }

  const auto &players = manager->GetByTag(shared::core::GameObjectType::PLAYER);
  if (players.empty()) { return; }

  const auto &worlds = manager->GetByTag(shared::core::GameObjectType::WORLD);
  if (worlds.empty() || !worlds.front()) { return; }

  const auto world_nav = worlds.front()->GetComponent<shared::core::component::WorldNavigation>();
  if (!world_nav) { return; }

  const size_t player_count = std::min<size_t>(2, players.size());

  for (uint32_t i = 0; i < count; ++i) {
    const auto &player_obj = players[i % player_count];
    const Vector2 spawn_pos = FindWalkableNear(player_obj, world_nav);
    auto enemy = BuildEnemyAt(spawn_pos);

    manager->Register(enemy);
  }
}

Vector2 WaveManager::FindWalkableNear(const std::shared_ptr<shared::core::GameObject> &player_obj,
  const std::shared_ptr<shared::core::component::WorldNavigation> &world_nav)
{
  if (!player_obj || world_nav == nullptr) { return Vector2{ 0.0F, 0.0F }; }

  const auto transform = player_obj->GetComponent<shared::core::component::Transform>();
  if (!transform) { return Vector2{ 0.0F, 0.0F }; }

  const Vector2 player_pos = transform->GetPosition();
  const auto &grid = world_nav->GetGrid();
  const int width = world_nav->GetWidth();
  const int height = world_nav->GetHeight();
  const int tile_size = world_nav->GetTileSize();
  if (grid.empty() || width <= 0 || height <= 0 || tile_size <= 0) { return Vector2{ 0.0F, 0.0F }; }

  const int tile_x = static_cast<int>(player_pos.x / static_cast<float>(tile_size));
  const int tile_y = static_cast<int>(player_pos.y / static_cast<float>(tile_size));

  constexpr int max_radius = 5;
  constexpr float min_distance = 100.0F;
  std::vector<Vector2> candidates;
  for (int dy = -max_radius; dy <= max_radius; ++dy) {
    for (int dx = -max_radius; dx <= max_radius; ++dx) {
      const int nx = tile_x + dx;
      const int ny = tile_y + dy;
      if (nx < 0 || ny < 0 || nx >= width || ny >= height) { continue; }

      const shared::ai::NavNode &node = grid[(ny * width) + nx];
      if (!node.is_walkable) { continue; }

      Vector2 const candidate_pos = { (node.position.x * static_cast<float>(tile_size))
                                        + (static_cast<float>(tile_size) * 0.5F),
        (node.position.y * static_cast<float>(tile_size)) + (static_cast<float>(tile_size) * 0.5F) };

      const float dist = Vector2Distance(player_pos, candidate_pos);
      if (dist < min_distance) { continue; }

      candidates.push_back(candidate_pos);
    }
  }

  if (!candidates.empty()) {
    const int idx = GetRandomValue(0, static_cast<int>(candidates.size()) - 1);
    return candidates[static_cast<size_t>(idx)];
  }

  return Vector2{ 0.0F, 0.0F };
}

std::shared_ptr<shared::core::GameObject> WaveManager::BuildEnemyAt(const Vector2 &spawn_pos)
{
  return shared::builder::GameObjectBuilder<shared::core::enemy::Enemy>()
    .Id(shared::utils::UUID::Generate())
    .AddTransform(spawn_pos)
    .AddSpeed(30.0F)
    .AddHealth(50.0F, 40.0F)
    .AddMovement()
    .AddColliderBox(GameContextType::Server, { 12.F, 12.F }, { -6.F, 0.F })
    .AddEventColliderBox(GameContextType::Server, { 32.F, 32.F }, { -16.F, -16.F })
    .AddEnemyAI()
    .NetRole(shared::core::NetRole::AUTHORITY)
    .Build();
}

}// namespace chroma::server::logic
