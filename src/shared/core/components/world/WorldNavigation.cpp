#include "chroma/shared/core/components/world/WorldNavigation.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/world/WorldSystem.h"
#include "chroma/shared/core/world/WorldSystem.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <raylib.h>
#include <vector>

namespace chroma::shared::core::component {
WorldNavigation::WorldNavigation() { type_ = ComponentType::WORLD_NAVIGATION; }

void WorldNavigation::Initialize()
{
  const auto game_object = GetGameObject();
  if (!game_object) { return; }

  world_system_ = game_object->GetComponent<WorldSystem>()->GetWorldSystem();
  width_ = world_system_->GetWidth();
  height_ = world_system_->GetHeight();
  tile_size_ = world_system_->GetTileSize();

  BuildGraph();
}

// NOLINTBEGIN
void WorldNavigation::Render()
{
  for (const auto &node : nav_grid_) {
    if (!node.is_walkable) {
      DrawLine(node.position.x * tile_size_,
        node.position.y * tile_size_,
        (node.position.x + 1) * tile_size_,
        (node.position.y + 1) * tile_size_,
        RED);
      DrawLine((node.position.x + 1) * tile_size_,
        node.position.y * tile_size_,
        node.position.x * tile_size_,
        (node.position.y + 1) * tile_size_,
        RED);
    } else {
      DrawPixel(
        (node.position.x * tile_size_) + tile_size_ / 2.0F, (node.position.y * tile_size_) + tile_size_ / 2.0F, GREEN);
    }
  }
}
// NOLINTEND

const std::vector<ai::NavNode> &WorldNavigation::GetGrid() const { return nav_grid_; }

int WorldNavigation::GetWidth() const { return width_; }

int WorldNavigation::GetTileSize() const { return tile_size_; }

int WorldNavigation::GetHeight() const { return height_; }

void WorldNavigation::AllocateGrid()
{
  const auto total = static_cast<size_t>(static_cast<long long>(width_) * static_cast<long long>(height_));
  nav_grid_.assign(total, ai::NavNode{});
}

void WorldNavigation::InitializeEmptyNodes()
{
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const auto idx =
        static_cast<size_t>((static_cast<long long>(y) * static_cast<long long>(width_)) + static_cast<long long>(x));
      ai::NavNode &n = nav_grid_[idx];
      n.position = { .x = static_cast<float>(x), .y = static_cast<float>(y) };
      n.is_walkable = true;
      n.neighbors.clear();
    }
  }
}

void WorldNavigation::ApplyColliders()
{
  const auto &colliders = world_system_->GetAllColliders();

  for (const world::CollisionRect &r : colliders) {
    const int start_x = std::max(0, static_cast<int>(static_cast<double>(r.rect.x) / static_cast<double>(tile_size_)));
    const int end_x = std::min(width_ - 1,
      static_cast<int>((static_cast<double>(r.rect.x) + static_cast<double>(r.rect.width) - 0.0001)
                       / static_cast<double>(tile_size_)));
    const int start_y = std::max(0, static_cast<int>(static_cast<double>(r.rect.y) / static_cast<double>(tile_size_)));
    const int end_y = std::min(height_ - 1,
      static_cast<int>((static_cast<double>(r.rect.y) + static_cast<double>(r.rect.height) - 0.0001)
                       / static_cast<double>(tile_size_)));

    for (int ty = start_y; ty <= end_y; ++ty) {
      for (int tx = start_x; tx <= end_x; ++tx) {
        const size_t idx = (static_cast<size_t>(ty) * static_cast<size_t>(width_)) + static_cast<size_t>(tx);
        nav_grid_[idx].is_walkable = false;
      }
    }
  }
}

// NOLINTBEGIN
void WorldNavigation::ComputeNeighbors()
{
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      ai::NavNode &node = nav_grid_[static_cast<size_t>(y * width_ + x)];
      if (!node.is_walkable) { continue; }

      if (y > 0) {
        const ai::NavNode &up = nav_grid_[static_cast<size_t>(((y - 1) * width_) + x)];
        if (up.is_walkable) {
          node.neighbors.emplace_back(Vector2{ static_cast<float>(x), static_cast<float>(y - 1) });
        }
      }
      if (y < height_ - 1) {
        const ai::NavNode &down = nav_grid_[static_cast<size_t>(((y + 1) * width_) + x)];
        if (down.is_walkable) {
          node.neighbors.emplace_back(Vector2{ static_cast<float>(x), static_cast<float>(y + 1) });
        }
      }
      if (x > 0) {
        const ai::NavNode &left = nav_grid_[static_cast<size_t>(y * width_ + (x - 1))];
        if (left.is_walkable) {
          node.neighbors.emplace_back(Vector2{ static_cast<float>(x - 1), static_cast<float>(y) });
        }
      }
      if (x < width_ - 1) {
        const ai::NavNode &right = nav_grid_[static_cast<size_t>(y * width_ + (x + 1))];
        if (right.is_walkable) {
          node.neighbors.emplace_back(Vector2{ static_cast<float>(x + 1), static_cast<float>(y) });
        }
      }
    }
  }
}
// NOLINTEND

void WorldNavigation::BuildGraph()
{
  if (!world_system_) { return; }

  AllocateGrid();
  InitializeEmptyNodes();
  ApplyColliders();
  ComputeNeighbors();
}
}// namespace chroma::shared::core::component