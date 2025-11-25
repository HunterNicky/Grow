#pragma once
#include "WorldGenerator.h"

#include <raylib.h>

namespace chroma::shared::core::world {
enum class TransitionType : uint8_t { None, Cliff, RampNorth, RampSouth, RampEast, RampWest };

enum class EdgeDirection : uint8_t {
  None = 0,
  North = 1,
  South = 2,
  East = 4,
  West = 8,
  NorthEast = 16,
  NorthWest = 32,
  SouthEast = 64,
  SouthWest = 128
};

inline EdgeDirection operator|(EdgeDirection a, EdgeDirection b)
{
  return static_cast<EdgeDirection>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline EdgeDirection &operator|=(EdgeDirection &a, const EdgeDirection b) { return a = a | b; }

inline bool operator&(EdgeDirection a, EdgeDirection b)
{
  return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

struct TileData
{
  int terrain_index = 0;
  int elevation = 0;
  TransitionType transition = TransitionType::None;
  EdgeDirection edges = EdgeDirection::None;
  bool is_ramp = false;
  int structure_index = -1;
};

struct CollisionRect
{
  Rectangle rect;
  int elevation;
  bool is_cliff;
};

struct RenderTile
{
  int world_x, world_y;
  int tile_size;
  const TileData *data;
  int render_priority;
};

class WorldSystem
{
public:
  WorldSystem();
  ~WorldSystem();

  bool Initialize(const std::string &config_filepath);

  [[nodiscard]] const TileData *GetTile(int wx, int wy) const;
  [[nodiscard]] int GetElevation(int wx, int wy) const;

  [[nodiscard]] int GetTileSize() const;
  [[nodiscard]] int GetWidth() const;
  [[nodiscard]] int GetHeight() const;

  [[nodiscard]] const WorldConfig &GetConfig() const;

  [[nodiscard]] std::vector<CollisionRect> GetColliders(Rectangle area) const;
  [[nodiscard]] std::vector<CollisionRect> GetAllColliders() const;

  [[nodiscard]] std::vector<RenderTile> GetVisibleTiles(Rectangle camera_bounds) const;
  [[nodiscard]] std::vector<RenderTile> GetRenderTile() const;

  void RenderDebugColliders() const;

private:
  WorldGenerator generator_;

  std::vector<TileData> tiles_;
  std::vector<CollisionRect> global_colliders_;

  std::unordered_map<int, int> terrain_elevations_;

  int width_ = 0;
  int height_ = 0;
  int tile_size_ = 16;

  void GenerateWorld();
  void CalculateTransitions();
  void PlaceRamps();
  void GenerateColliders();

  [[nodiscard]] EdgeDirection CalculateEdges(int wx, int wy, int elev) const;
  [[nodiscard]] int GetElevationForTerrain(int terrain_index) const;
};
}// namespace chroma::shared::world