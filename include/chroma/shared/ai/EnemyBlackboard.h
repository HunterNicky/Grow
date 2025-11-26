#pragma once
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/ai/Astar.h"
#include <memory>
#include <raylib.h>

namespace chroma::shared::ai {

struct EnemyBlackboard
{
  std::weak_ptr<core::GameObject> owner;
  std::weak_ptr<core::GameObject> target;

  Vector2 target_position = { 0.0F, 0.0F };
  float attack_range = 30.0F;
  float sight_range = 300.0F;
  const std::vector<NavNode> *nav_grid = nullptr;
  int grid_width = 0;
  int grid_height = 0;
  int tile_size = 0;
  std::vector<Vector2> current_path;
  std::size_t path_index = 0;
};
}// namespace chroma::shared::ai