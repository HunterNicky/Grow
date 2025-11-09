#pragma once
#include "TextureAtlas.h"

#include <memory>

namespace chroma::client::render {
struct SpriteDrawParams
{
  Vector2 position = { 0, 0 };
  Vector2 origin = { 0.5F, 0.5F };
  Vector2 scale = { 1.0F, 1.0F };
  float rotation = 0.0F;
  Color tint = WHITE;
  bool flip_x = false;
  bool flip_y = false;
  Rectangle subregion = { 0, 0, 0, 0 };
  Vector2 subregion_offset = { 0, 0 };
};

class SpriteRenderer
{
public:
  explicit SpriteRenderer(std::shared_ptr<TextureAtlas> atlas_manager);

  void DrawSprite(const std::string &sprite_id, const SpriteDrawParams &params) const noexcept;
  void DrawSprite(const AtlasRegion &region, const SpriteDrawParams &params) const noexcept;

private:
  std::shared_ptr<TextureAtlas> atlas_manager_;
};
}// namespace chroma::client::render