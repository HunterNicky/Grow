#pragma once

#include "chroma/shared/ai/Astar.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/world/WorldSystem.h"

#include <memory>
#include <vector>

namespace chroma::shared::core::component {
class WorldNavigation final : public Component
{
public:
  WorldNavigation();
  ~WorldNavigation() override = default;

  void Initialize();
  void Render() override;

  [[nodiscard]] const std::vector<ai::NavNode> &GetGrid() const;

  [[nodiscard]] int GetWidth() const;
  int GetTileSize() const;
  [[nodiscard]] int GetHeight() const;

private:
  void BuildGraph();
  void AllocateGrid();
  void InitializeEmptyNodes();
  void ApplyColliders();
  void ComputeNeighbors();

  std::shared_ptr<world::WorldSystem> world_system_;
  std::vector<ai::NavNode> nav_grid_;
  int width_ = 0;
  int height_ = 0;
  int tile_size_ = 0;
};
}// namespace chroma::shared::core::component