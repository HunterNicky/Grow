#pragma once

#include <FastNoiseLite.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace chroma::shared::core::world {
using json = nlohmann::json;

struct NoiseConfig
{
  std::string type = "OpenSimplex2";
  float frequency = 0.01F;
  int octaves = 3;
  float lacunarity = 2.0F;
  float gain = 0.5F;
  std::string fractal_type = "FBm";
};

struct TerrainLayer
{
  std::string id;
  float cutoff;
  int tile_index;
  int elevation;
};

struct StructureConfig
{
  std::string id;
  float density;
  int tile_index;
};

struct WorldSize
{
  int width;
  int height;
};

struct WorldConfig
{
  int seed;
  int tile_size;
  WorldSize world_size;
  NoiseConfig terrain_noise;
  NoiseConfig structure_noise;
  std::vector<TerrainLayer> layers;
  std::vector<StructureConfig> structures;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NoiseConfig, type, frequency, octaves, lacunarity, gain, fractal_type)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TerrainLayer, id, cutoff, tile_index, elevation)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StructureConfig, id, density, tile_index)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WorldSize, width, height)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WorldConfig,
  seed,
  tile_size,
  world_size,
  terrain_noise,
  structure_noise,
  layers,
  structures)

class WorldGenerator
{
public:
  WorldGenerator() = default;
  ~WorldGenerator() = default;

  bool LoadConfigFromFile(const std::string &filepath);
  [[nodiscard]] const WorldConfig &GetConfig() const;

  [[nodiscard]] float GetTerrainNoiseAtPosition(int x, int y) const;
  [[nodiscard]] float GetStructureNoiseAtPosition(int x, int y) const;

  [[nodiscard]] bool IsLoaded() const;

private:
  FastNoiseLite terrain_noise_;
  FastNoiseLite structure_noise_;
  WorldConfig config_;
  bool load_;

  static void ConfigureNoise(FastNoiseLite &noise, const NoiseConfig &config, int seed);
};
}// namespace chroma::shared::world