#pragma once

#include "chroma/client/render/Renderer.h"
#include "chroma/shared/render/RenderBridge.h"
#include <unordered_map>
#include <unordered_set>

#include <memory>
#include <string>

namespace chroma::client::render {

class RenderBridgeImpl final : public shared::render::IRenderBridge
{
public:
  explicit RenderBridgeImpl(Renderer *renderer);
  void DrawSprite(const std::string &sprite_id,
    Vector2 position,
    Vector2 scale,
    float rotation,
    Color tint,
    bool flip_x,
    bool flip_y,
    Vector2 origin,
    Vector2 offset) override;

  void DrawAnimation(const shared::core::component::SpriteAnimation &anim,
    Vector2 position,
    Vector2 scale,
    float rotation,
    Color tint,
    bool flip_x,
    bool flip_y,
    Vector2 origin) override;

  void LoadSprite(const std::string &filepath) override;

  void CameraSetMode(shared::render::CameraMode mode) override;
  void CameraSetTarget(Vector2 target) override;
  void CameraSetBounds(Rectangle bounds) override;
  void CameraSetZoom(float zoom) override;
  void CameraSetSmoothness(float smoothness) override;
  void CameraUpdate(float delta_time) override;
  void CameraSetDeadzone(Vector2 size) override;

  const Rectangle GetActiveCameraBounds() const override;

private:
  Renderer *renderer_;
  std::unordered_map<const void *, animation::AnimationController> controllers_;
  std::unordered_map<const void *, std::string> current_anim_;
  std::unordered_set<const void *> initialized_;
};

}// namespace chroma::client::render
