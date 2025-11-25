#include "chroma/shared/core/components/world/WorldNavigation.h"
#include "chroma/shared/core/world/WorldSystem.h"

#include <memory>
#include <raylib.h>
#include <vector>

namespace chroma::shared::core::component {

void component::WorldNavigation::Initialize(std::shared_ptr<world::WorldSystem> world_system)
{
  world_system_ = world_system;
  width_ = world_system_->GetWidth();
  height_ = world_system_->GetHeight();
  tile_size_ = world_system_->GetTileSize();

  BuildGraph();
}

bool component::WorldNavigation::IsPositionWalkable(Vector2 world_pos) const
{
  int x = (int)(world_pos.x / tile_size_);
  int y = (int)(world_pos.y / tile_size_);

  if (x < 0 || x >= width_ || y < 0 || y >= height_) return false;

  return nav_grid_[y * width_ + x].is_walkable;
}

void component::WorldNavigation::Render()
{
  for (const auto &node : nav_grid_) {
    if (!node.is_walkable) {
      DrawLine(node.x * tile_size_, node.y * tile_size_, (node.x + 1) * tile_size_, (node.y + 1) * tile_size_, RED);
      DrawLine((node.x + 1) * tile_size_, node.y * tile_size_, node.x * tile_size_, (node.y + 1) * tile_size_, RED);
    } else {
      DrawPixel((node.x * tile_size_) + tile_size_ / 2, (node.y * tile_size_) + tile_size_ / 2, GREEN);
    }
  }
}

const std::vector<NavNode> &component::WorldNavigation::GetGrid() const { return nav_grid_; }

int component::WorldNavigation::GetWidth() const { return width_; }

int component::WorldNavigation::GetHeight() const { return height_; }

void component::WorldNavigation::BuildGraph()
{
  if (!world_system_) return;

  nav_grid_.clear();
  nav_grid_.resize(width_ * height_);

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      int index = (y * width_) + x;
      NavNode &node = nav_grid_[index];
      node.x = x;
      node.y = y;

      float margin = 2.0f;
      Rectangle check_rect = { (float)x * tile_size_ + margin,
        (float)y * tile_size_ + margin,
        (float)tile_size_ - (margin * 2),
        (float)tile_size_ - (margin * 2) };

      auto colliders = world_system_->GetColliders(check_rect);

      node.is_walkable = colliders.empty();
    }
  }

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      int index = (y * width_) + x;
      NavNode &node = nav_grid_[index];

      if (!node.is_walkable) continue;

      const int dx[] = { 0, 0, 1, -1 };
      const int dy[] = { -1, 1, 0, 0 };

      for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < width_ && ny >= 0 && ny < height_) {
          int neighbor_index = (ny * width_) + nx;

          if (nav_grid_[neighbor_index].is_walkable) { node.neighbors.push_back(Vector2{ (float)nx, (float)ny }); }
        }
      }
    }
  }
}

}// namespace chroma::shared::core::component