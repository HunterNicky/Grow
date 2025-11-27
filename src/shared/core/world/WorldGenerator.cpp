#include "chroma/shared/core/world/WorldGenerator.h"

#include <FastNoiseLite.h>
#include <algorithm>
#include <fstream>
#include <raylib.h>
#include <string>

namespace chroma::shared::core::world {
bool WorldGenerator::LoadConfigFromFile(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open()) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_ERROR, "Failed to open world config file: %s", filepath.c_str());
    return false;
  }
  json j;
  file >> j;
  config_ = j.get<WorldConfig>();

  std::ranges::sort(config_.layers, [](const TerrainLayer &a, const TerrainLayer &b) { return a.cutoff < b.cutoff; });
  ConfigureNoise(terrain_noise_, config_.terrain_noise, config_.seed);
  ConfigureNoise(structure_noise_, config_.structure_noise, config_.seed + 1);

  load_ = true;
  return true;
}

const WorldConfig &WorldGenerator::GetConfig() const { return config_; }

float WorldGenerator::GetTerrainNoiseAtPosition(const int x, const int y) const
{
  return terrain_noise_.GetNoise(static_cast<float>(x), static_cast<float>(y));
}
float WorldGenerator::GetStructureNoiseAtPosition(const int x, const int y) const
{
  return structure_noise_.GetNoise(static_cast<float>(x), static_cast<float>(y));
}

bool WorldGenerator::IsLoaded() const { return load_; }

void WorldGenerator::ConfigureNoise(FastNoiseLite &noise, const NoiseConfig &config, const int seed)
{
  noise.SetSeed(seed);
  noise.SetFrequency(config.frequency);
  noise.SetFractalOctaves(config.octaves);
  noise.SetFractalLacunarity(config.lacunarity);
  noise.SetFractalGain(config.gain);

  if (config.type == "Perlin") {
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
  } else if (config.type == "Simplex") {
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);
  } else {
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Cellular);
  }

  if (config.fractal_type == "FBm") {
    noise.SetFractalType(FastNoiseLite::FractalType::FractalType_FBm);
  } else if (config.fractal_type == "Ridged") {
    noise.SetFractalType(FastNoiseLite::FractalType::FractalType_Ridged);
  } else {
    noise.SetFractalType(FastNoiseLite::FractalType::FractalType_PingPong);
  }
}
}// namespace chroma::shared::core::world