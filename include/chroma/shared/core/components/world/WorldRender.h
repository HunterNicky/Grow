#pragma once
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/world/WorldSystem.h"

#include <nlohmann/json.hpp>

namespace chroma::shared::core::component {
using json = nlohmann::json;

struct SpriteParam
{
  float x;
  float y;
  float width;
  float height;
};

struct SpriteSet
{
  std::string sprite_id;
  SpriteParam south;
  SpriteParam north;
  SpriteParam east;
  SpriteParam west;
  SpriteParam center;
  SpriteParam south_east;
  SpriteParam south_west;
  SpriteParam north_east;
  SpriteParam north_west;
  SpriteParam ramp_north;
  SpriteParam ramp_south;
  SpriteParam ramp_east;
  SpriteParam ramp_west;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteParam, x, y, width, height)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SpriteSet,
  sprite_id,
  south,
  north,
  east,
  west,
  center,
  south_east,
  south_west,
  north_east,
  north_west,
  ramp_north,
  ramp_south,
  ramp_east,
  ramp_west)

class WorldRender final : public Component
{
public:
  WorldRender();
  ~WorldRender() override = default;
  void Render() override;

  void SetRenderTile(const std::vector<world::RenderTile> &tiles);

  void LoadConfigFromFile(const std::string &filepath);
  void Initialize(const std::string &path);

private:
  struct DrawParams
  {
    std::string sprite_id;
    Vector2 position;
    Vector2 scale;
    Vector2 origin;
    Vector2 offset;
    bool flip_x;
    bool flip_y;
    float rotation;
  };

  std::vector<world::RenderTile> tiles_;
  std::vector<DrawParams> draw_params_;
  SpriteSet sprite_set_;
};
}// namespace chroma::shared::core::component