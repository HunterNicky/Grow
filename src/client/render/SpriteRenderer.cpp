#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/TextureAtlas.h"

#include <cmath>
#include <cstdlib>
#include <memory>
#include <optional>
#include <raylib.h>
#include <string>
#include <utility>

namespace chroma::client::render {
SpriteRenderer::SpriteRenderer(std::shared_ptr<TextureAtlas> atlas_manager) : atlas_manager_(std::move(atlas_manager))
{}

void SpriteRenderer::DrawSprite(const std::string &sprite_id, const SpriteDrawParams &params) const noexcept
{
  const std::optional<const AtlasRegion *> region = atlas_manager_->GetRegion(sprite_id);
  if (!region) { return; }
  DrawSprite(**region, params);
}

void SpriteRenderer::DrawSprite(const AtlasRegion &region, const SpriteDrawParams &params) const noexcept
{
  const Texture2D *texture = atlas_manager_->GetTexture();
  Rectangle source_rect = region.source;

  if (params.subregion.width > 0 && params.subregion.height > 0) {
    source_rect.x += params.subregion.x;
    source_rect.y += params.subregion.y;
    source_rect.width = params.subregion.width;
    source_rect.height = params.subregion.height;
  }

  if (params.flip_x) { source_rect.width *= -1; }
  if (params.flip_y) { source_rect.height *= -1; }

  const float draw_w =
    (params.subregion.width > 0 ? params.subregion.width : static_cast<float>(std::abs(region.width))) * params.scale.x;
  const float draw_h =
    (params.subregion.height > 0 ? params.subregion.height : static_cast<float>(std::abs(region.height)))
    * params.scale.y;

  const Vector2 origin = { params.origin.x * draw_w, params.origin.y * draw_h };

  const Vector2 draw_pos = { params.position.x + params.subregion_offset.x,
    params.position.y + params.subregion_offset.y };

  DrawTexturePro(
    *texture, source_rect, { draw_pos.x, draw_pos.y, draw_w, draw_h }, origin, params.rotation, params.tint);
}
}// namespace chroma::client::render
