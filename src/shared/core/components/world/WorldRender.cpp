#include "chroma/shared/core/components/world/WorldRender.h"
#include "chroma/shared/render/RenderBridge.h"

#include <fstream>

namespace chroma::shared::core::component {
WorldRender::WorldRender() : sprite_set_() {}

void WorldRender::Render()
{
  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }

  const auto camera = bridge->GetActiveCameraBounds();
  for (const auto &params : draw_params_) {
    if (!(params.position.x >= camera.x && params.position.x <= camera.x + camera.width && params.position.y >= camera.y
          && params.position.y <= camera.y + camera.height)) {
      continue;
    }

    bridge->DrawSprite(params.sprite_id,
      params.position,
      params.scale,
      params.rotation,
      WHITE,
      params.flip_x,
      params.flip_y,
      params.origin,
      params.offset);
  }
}
void WorldRender::SetRenderTile(const std::vector<world::RenderTile> &tiles) { tiles_ = tiles; }

void WorldRender::LoadConfigFromFile(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open()) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_ERROR, "Failed to open world render config file: %s", filepath.c_str());
    return;
  }
  json j;
  file >> j;
  sprite_set_ = j.get<SpriteSet>();
}

void WorldRender::Initialize(const std::string &path)
{
  LoadConfigFromFile(path);
  if (!tiles_.data()) { TraceLog(LOG_WARNING, "WorldRender: No tiles set during initialization."); }

  auto selectSprite = [&](const world::RenderTile &tile) -> SpriteParam {
    const auto *data = tile.data;
    if (!data) return sprite_set_.center;

    switch (data->transition) {
    case world::TransitionType::RampNorth:
      return sprite_set_.ramp_north;
    case world::TransitionType::RampSouth:
      return sprite_set_.ramp_south;
    case world::TransitionType::RampEast:
      return sprite_set_.ramp_east;
    case world::TransitionType::RampWest:
      return sprite_set_.ramp_west;
    default:
      break;
    }

    const auto edges = data->edges;
    if (edges & world::EdgeDirection::NorthEast) return sprite_set_.north_east;
    if (edges & world::EdgeDirection::NorthWest) return sprite_set_.north_west;
    if (edges & world::EdgeDirection::SouthEast) return sprite_set_.south_east;
    if (edges & world::EdgeDirection::SouthWest) return sprite_set_.south_west;

    if (edges & world::EdgeDirection::North) return sprite_set_.north;
    if (edges & world::EdgeDirection::South) return sprite_set_.south;
    if (edges & world::EdgeDirection::East) return sprite_set_.east;
    if (edges & world::EdgeDirection::West) return sprite_set_.west;

    return sprite_set_.center;
  };

  std::vector<DrawParams> draw_params;
  draw_params.reserve(tiles_.size());

  for (const auto &tile : tiles_) {
    const SpriteParam sprite = selectSprite(tile);

    DrawParams params{ .sprite_id = sprite_set_.sprite_id,
      .position = { static_cast<float>(tile.world_x), static_cast<float>(tile.world_y) },
      .scale = { 1.0f, 1.0f },
      .origin = { 0.0f, 0.0f },

      .offset = { sprite.x, sprite.y },

      .flip_x = false,
      .flip_y = false,
      .rotation = 0.0f };

    draw_params.push_back(params);
  }

  draw_params_ = std::move(draw_params);
}


// void WorldRender::Initialize(const std::string &path)
// {
//   LoadConfigFromFile(path);
//   if (!tiles_.data()) { TraceLog(LOG_WARNING, "WorldRender: No tiles set during initialization."); }
//
//   std::vector<DrawParams> draw_params;
//   for (const auto &tile : tiles_) {
//
//     DrawParams params{ .sprite_id = sprite_set_.sprite_id,
//       .position = { static_cast<float>(tile.world_x), static_cast<float>(tile.world_y) },
//       .scale = { 1.0F, 1.0F },
//       .origin = { 0.0F, 0.0F },
//       .offset = { static_cast<float>(tile.tile_size), static_cast<float>(tile.tile_size) },
//       .flip_x = false,
//       .flip_y = false,
//       .rotation = 0.0F };
//     draw_params.push_back(params);
//   }
//   draw_params_ = draw_params;
// }

}// namespace chroma::shared::core::component