#include "chroma/shared/core/components/world/WorldRender.h"
#include "chroma/shared/core/world/WorldSystem.h"
#include "chroma/shared/render/RenderBridge.h"

#include <fstream>
#include <raylib.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace chroma::shared::core::component {
WorldRender::WorldRender() : sprite_set_() {}

void WorldRender::Render()
{
  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }

  const Rectangle camera = bridge->GetActiveCameraBounds();
  static bool t_pressed = false;

  if (IsKeyPressed(KEY_T)) { t_pressed = !t_pressed; }

  for (const auto &params : draw_params_) {
    const float sprite_width = params.subregion.width * params.scale.x;
    const float sprite_height = params.subregion.height * params.scale.y;
    const float world_x = params.position.x - (params.origin.x * params.scale.x);
    const float world_y = params.position.y - (params.origin.y * params.scale.y);

    const Rectangle sprite_rect = { world_x, world_y, sprite_width, sprite_height };

    if (!CheckCollisionRecs(camera, sprite_rect)) { continue; }
    if (t_pressed) {
      if (params.draw_background) {
        bridge->DrawSprite(params.sprite_id,
          params.position,
          params.scale,
          params.rotation,
          WHITE,
          params.flip_x,
          params.flip_y,
          params.origin,
          params.offset,
          { sprite_set_.center.x, sprite_set_.center.y, sprite_set_.center.width, sprite_set_.center.height },
          render::RenderLayer::Ground,
          0);
      }
      bridge->DrawSprite(params.sprite_id,
        params.position,
        params.scale,
        params.rotation,
        WHITE,
        params.flip_x,
        params.flip_y,
        params.origin,
        params.offset,
        params.subregion,
        render::RenderLayer::Ground,
        1);
    }
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

// NOLINTBEGIN
void WorldRender::Initialize(const std::string &path)
{
  LoadConfigFromFile(path);
  if (tiles_.data() == nullptr) { TraceLog(LOG_WARNING, "WorldRender: No tiles set during initialization."); }

  auto selectSprite = [&](const world::RenderTile &tile) -> std::tuple<SpriteParam, bool> {
    const auto *data = tile.data;
    if (!data) return std::tuple<SpriteParam, bool>(sprite_set_.center, false);

    switch (data->transition) {
    case world::TransitionType::RampNorth:
      return std::tuple<SpriteParam, bool>(sprite_set_.ramp_south, false);
    case world::TransitionType::RampSouth:
      return std::tuple<SpriteParam, bool>(sprite_set_.ramp_north, false);
    case world::TransitionType::RampEast:
      return std::tuple<SpriteParam, bool>(sprite_set_.ramp_west, false);
    case world::TransitionType::RampWest:
      return std::tuple<SpriteParam, bool>(sprite_set_.ramp_east, false);
    default:
      break;
    }

    const auto edges = data->edges;

    const bool n = (edges & world::EdgeDirection::North);
    const bool s = (edges & world::EdgeDirection::South);
    const bool e = (edges & world::EdgeDirection::East);
    const bool w = (edges & world::EdgeDirection::West);

    // if (n && e) return sprite_set_.north_east;
    if (n && e) return std::tuple<SpriteParam, bool>(sprite_set_.north_east, true);
    if (n && w) return std::tuple<SpriteParam, bool>(sprite_set_.north_west, true);
    if (s && e) return std::tuple<SpriteParam, bool>(sprite_set_.south_east, true);
    if (s && w) return std::tuple<SpriteParam, bool>(sprite_set_.south_west, true);

    if (n) return std::tuple<SpriteParam, bool>(sprite_set_.north, true);
    if (s) return std::tuple<SpriteParam, bool>(sprite_set_.south, true);
    if (e) return std::tuple<SpriteParam, bool>(sprite_set_.east, true);
    if (w) return std::tuple<SpriteParam, bool>(sprite_set_.west, true);

    if (edges & world::EdgeDirection::NorthEast) return std::tuple<SpriteParam, bool>(sprite_set_.south_west, true);
    if (edges & world::EdgeDirection::NorthWest) return std::tuple<SpriteParam, bool>(sprite_set_.south_east, true);
    if (edges & world::EdgeDirection::SouthEast) return std::tuple<SpriteParam, bool>(sprite_set_.north_west, true);
    if (edges & world::EdgeDirection::SouthWest) return std::tuple<SpriteParam, bool>(sprite_set_.north_east, true);

    return std::tuple<SpriteParam, bool>(sprite_set_.center, false);
  };
  // NOLINTEND

  std::vector<DrawParams> draw_params;
  draw_params.reserve(tiles_.size());

  for (const auto &tile : tiles_) {
    std::tuple<SpriteParam, bool> sprite_bool = selectSprite(tile);
    const SpriteParam sprite = std::get<0>(sprite_bool);

    const DrawParams params{
      .sprite_id = sprite_set_.sprite_id,
      .position = { static_cast<float>(tile.world_x * tile.tile_size),
        static_cast<float>(tile.world_y * tile.tile_size) },

      .scale = { 1.0F, 1.0F },
      .origin = { 0.0F, 0.0F },
      .offset = { 0.0F, 0.0F },
      .flip_x = false,
      .flip_y = false,
      .rotation = 0.0F,
      .subregion = { sprite.x, sprite.y, sprite.width, sprite.height },
      .draw_background = std::get<1>(sprite_bool),
    };

    draw_params.push_back(params);
  }

  draw_params_ = std::move(draw_params);
}

}// namespace chroma::shared::core::component