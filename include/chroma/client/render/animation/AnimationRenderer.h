#pragma once
#include "AnimationController.h"
#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/TextureAtlas.h"

#include <memory>

namespace chroma::client::render::animation {
class AnimationRenderer
{
public:
  explicit AnimationRenderer(std::shared_ptr<TextureAtlas> atlas_manager,
    std::shared_ptr<SpriteRenderer> sprite_renderer);

  void DrawAnimation(const AnimationController &controller, const SpriteDrawParams &params) const;

private:
  std::shared_ptr<TextureAtlas> atlas_manager_;
  std::shared_ptr<SpriteRenderer> sprite_renderer_;
};
}// namespace chroma::client::render::animation