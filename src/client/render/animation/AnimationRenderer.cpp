#include "chroma/client/render/animation/AnimationRenderer.h"
#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/TextureAtlas.h"
#include "chroma/client/render/animation/AnimationController.h"
#include "chroma/client/render/animation/AnimationState.h"

#include <memory>
#include <utility>

namespace chroma::client::render::animation {
AnimationRenderer::AnimationRenderer(std::shared_ptr<TextureAtlas> atlas_manager,
  std::shared_ptr<SpriteRenderer> sprite_renderer)
  : atlas_manager_(std::move(atlas_manager)), sprite_renderer_(std::move(sprite_renderer))
{}

void AnimationRenderer::DrawAnimation(const AnimationController &controller, const SpriteDrawParams &params) const
{
  const AnimationFrame *frame = controller.GetCurrentFrame();
  if ((frame == nullptr) || !atlas_manager_ || !sprite_renderer_) { return; }

  SpriteDrawParams draw_params = params;
  draw_params.position.x += frame->offset.x;
  draw_params.position.y += frame->offset.y;
  if (frame->subregion.width > 0 && frame->subregion.height > 0) {
    draw_params.subregion = frame->subregion;
    draw_params.subregion_offset = frame->subregion_offset;
  }
  sprite_renderer_->DrawSprite(frame->sprite_id, draw_params);
}
}// namespace chroma::client::render::animation
