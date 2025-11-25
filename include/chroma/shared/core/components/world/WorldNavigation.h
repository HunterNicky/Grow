#pragma once

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/world/WorldSystem.h"

#include <memory>
#include <raylib.h>
#include <vector>

namespace chroma::shared::core::component {
struct NavNode
{
  int x = 0, y = 0;
  bool is_walkable = false;
  float cost = 1.0f;
  std::vector<Vector2> neighbors = {};
};

class WorldNavigation : public Component
{
public:
  WorldNavigation() = default;
  ~WorldNavigation() override = default;

  void Initialize(std::shared_ptr<world::WorldSystem> world_system);

  bool IsPositionWalkable(Vector2 world_pos) const;

  void Render() override;

  const std::vector<NavNode> &GetGrid() const;
  int GetWidth() const;
  int GetHeight() const;

private:
  void BuildGraph();

  std::shared_ptr<world::WorldSystem> world_system_;
  std::vector<NavNode> nav_grid_;
  int width_ = 0;
  int height_ = 0;
  int tile_size_ = 0;
};
}// namespace chroma::shared::core::component