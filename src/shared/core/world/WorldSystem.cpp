#include "chroma/shared/core/world/WorldSystem.h"
#include "chroma/shared/core/world/WorldGenerator.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <raylib.h>
#include <string>
#include <vector>

namespace chroma::shared::core::world {
WorldSystem::WorldSystem() = default;

WorldSystem::~WorldSystem() = default;

bool WorldSystem::Initialize(const std::string &config_filepath)
{
  if (!generator_.LoadConfigFromFile(config_filepath)) { return false; }

  const WorldConfig &config = generator_.GetConfig();
  tile_size_ = config.tile_size;
  width_ = config.world_size.width;
  height_ = config.world_size.height;

  terrain_elevations_.clear();
  for (const auto &layer : config.layers) { terrain_elevations_[layer.tile_index] = layer.elevation; }

  GenerateWorld();
  CalculateTransitions();
  PlaceRamps();
  GenerateColliders();

  return true;
}

const TileData *WorldSystem::GetTile(const int wx, const int wy) const
{
  if (wx < 0 || wy < 0 || wx >= width_ || wy >= height_) { return nullptr; }
  return &tiles_[(wy * width_) + wx];
}

int WorldSystem::GetElevation(const int wx, const int wy) const
{
  const auto *tile = GetTile(wx, wy);
  return (tile != nullptr) ? tile->elevation : 0;
}

int WorldSystem::GetTileSize() const { return tile_size_; }

int WorldSystem::GetWidth() const { return width_; }

int WorldSystem::GetHeight() const { return height_; }

const WorldConfig &WorldSystem::GetConfig() const { return generator_.GetConfig(); }

std::vector<CollisionRect> WorldSystem::GetColliders(const Rectangle area) const
{
  std::vector<CollisionRect> result;

  for (const auto &col : global_colliders_) {
    if (CheckCollisionRecs(col.rect, area)) { result.push_back(col); }
  }
  return result;
}

std::vector<CollisionRect> WorldSystem::GetAllColliders() const { return global_colliders_; }

std::vector<RenderTile> WorldSystem::GetVisibleTiles(const Rectangle camera_bounds) const
{
  std::vector<RenderTile> result;

  const int start_x = std::max(0, static_cast<int>(camera_bounds.x / static_cast<float>(tile_size_)) - 1);
  const int start_y = std::max(0, static_cast<int>(camera_bounds.y / static_cast<float>(tile_size_)) - 1);
  const int end_x = std::min(
    width_ - 1, static_cast<int>((camera_bounds.x + camera_bounds.width) / static_cast<float>(tile_size_)) + 1);
  const int end_y = std::min(
    height_ - 1, static_cast<int>((camera_bounds.y + camera_bounds.height) / static_cast<float>(tile_size_)) + 1);

  for (int y = start_y; y <= end_y; ++y) {
    for (int x = start_x; x <= end_x; ++x) {
      const TileData *tile = GetTile(x, y);
      if (tile != nullptr) {
        result.push_back({ .world_x = x,
          .world_y = y,
          .tile_size = tile_size_,
          .data = tile,
          .render_priority = (tile->elevation * 10000) + y });
      }
    }
  }

  std::ranges::sort(
    result, [](const RenderTile &a, const RenderTile &b) { return a.render_priority < b.render_priority; });

  return result;
}

std::vector<RenderTile> WorldSystem::GetRenderTile() const
{
  std::vector<RenderTile> result;

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const TileData *tile = GetTile(x, y);
      if (tile != nullptr) {
        result.push_back({ .world_x = x,
          .world_y = y,
          .tile_size = tile_size_,
          .data = tile,
          .render_priority = (tile->elevation * 10000) + y });
      }
    }
  }

  std::ranges::sort(
    result, [](const RenderTile &a, const RenderTile &b) { return a.render_priority < b.render_priority; });

  return result;
}

void WorldSystem::RenderDebugColliders() const
{
  for (const auto &col : global_colliders_) {
    if (col.elevation == 0) { DrawRectangleRec(col.rect, BLUE); }
    if (col.elevation == 1) { DrawRectangleRec(col.rect, GREEN); }
    if (col.elevation == 2) { DrawRectangleRec(col.rect, GRAY); }
  }
}

void WorldSystem::GenerateWorld()
{
  const auto size = static_cast<size_t>(width_) * static_cast<size_t>(height_);
  tiles_.resize(size);

  std::vector<float> noise_map(size);
  float min_val = std::numeric_limits<float>::max();
  float max_val = std::numeric_limits<float>::lowest();

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const float val = generator_.GetTerrainNoiseAtPosition(x, y);

      noise_map[(y * width_) + x] = val;
      min_val = std::min(val, min_val);
      max_val = std::max(val, max_val);
    }
  }

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const float raw = noise_map[(y * width_) + x];

      const float normalized = (raw - min_val) / (max_val - min_val);

      int terrain_idx = 0;
      const auto &layers = generator_.GetConfig().layers;

      bool found = false;
      for (const auto &layer : layers) {
        if (normalized <= layer.cutoff) {
          terrain_idx = layer.tile_index;
          found = true;
          break;
        }
      }
      if (!found) { terrain_idx = layers.back().tile_index; }

      TileData &tile = tiles_[(y * width_) + x];
      tile.terrain_index = terrain_idx;

      tile.elevation = GetElevationForTerrain(terrain_idx);
    }
  }

  CalculateTransitions();
  GenerateColliders();
}

void WorldSystem::CalculateTransitions()
{
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      TileData &tile = tiles_[(y * width_) + x];
      tile.edges = CalculateEdges(x, y, tile.elevation);

      if (static_cast<uint8_t>(tile.edges) != 0) { tile.transition = TransitionType::Cliff; }
    }
  }
}

void WorldSystem::PlaceRamps()
{
  constexpr int ramp_interval = 20;
  int counter = 0;

  for (int y = 1; y < height_ - 1; ++y) {
    for (int x = 1; x < width_ - 1; ++x) {
      TileData &tile = tiles_[(y * width_) + x];
      if (tile.transition != TransitionType::Cliff) { continue; }

      if (++counter >= ramp_interval) {
        if (tile.edges & EdgeDirection::South) {
          tile.transition = TransitionType::RampSouth;
        } else if (tile.edges & EdgeDirection::North) {
          tile.transition = TransitionType::RampNorth;
        } else if (tile.edges & EdgeDirection::East) {
          tile.transition = TransitionType::RampEast;
        } else if (tile.edges & EdgeDirection::West) {
          tile.transition = TransitionType::RampWest;
        }

        if (tile.transition != TransitionType::Cliff) {
          tile.is_ramp = true;
          counter = 0;
        }
      }
    }
  }
}

void WorldSystem::GenerateColliders()
{
  global_colliders_.clear();
  const auto ts = static_cast<float>(tile_size_);

  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      const TileData &tile = tiles_[(y * width_) + x];
      const float px = static_cast<float>(x) * ts;
      const float py = static_cast<float>(y) * ts;

      if (tile.transition == TransitionType::Cliff) {
        const float edge_thick = ts * 0.25F;
        // const float edge_thick = ts;

        if (tile.edges & EdgeDirection::North) {
          global_colliders_.push_back(
            { .rect = { px, py, ts, edge_thick }, .elevation = tile.elevation, .is_cliff = true });
        }

        if (tile.edges & EdgeDirection::South) {
          global_colliders_.push_back(
            { .rect = { px, py + ts - edge_thick, ts, edge_thick }, .elevation = tile.elevation, .is_cliff = true });
        }

        if (tile.edges & EdgeDirection::East) {
          global_colliders_.push_back(
            { .rect = { px + ts - edge_thick, py, edge_thick, ts }, .elevation = tile.elevation, .is_cliff = true });
        }

        if (tile.edges & EdgeDirection::West) {
          global_colliders_.push_back(
            { .rect = { px, py, edge_thick, ts }, .elevation = tile.elevation, .is_cliff = true });
        }
      }
    }
  }
}

EdgeDirection WorldSystem::CalculateEdges(const int wx, const int wy, const int elev) const
{
  auto result = EdgeDirection::None;

  auto check = [&](const int dx, const int dy, const EdgeDirection dir) {
    const int nx = wx + dx;
    const int ny = wy + dy;

    if (nx < 0 || ny < 0 || nx >= width_ || ny >= height_) { return; }

    const int neighbor_elev = GetElevation(nx, ny);

    if (neighbor_elev < elev) { result |= dir; }
  };

  check(0, -1, EdgeDirection::North);
  check(0, 1, EdgeDirection::South);
  check(1, 0, EdgeDirection::East);
  check(-1, 0, EdgeDirection::West);

  return result;
}

int WorldSystem::GetElevationForTerrain(const int terrain_index) const
{
  const auto it = terrain_elevations_.find(terrain_index);
  return (it != terrain_elevations_.end()) ? it->second : 0;
}

}// namespace chroma::shared::core::world