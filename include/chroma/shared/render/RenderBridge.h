#pragma once

#include <memory>
#include <raylib.h>
#include <string>

#include "chroma/shared/core/components/SpriteAnimation.h"

namespace chroma::shared::render {
enum class CameraMode : uint8_t { Free = 0, Follow = 1, FollowSmooth = 2, Bounded = 3 };

enum class RenderLayer : uint16_t {
  Background = 0,
  Ground = 100,
  GroundDecoration = 200,
  Entities = 300,
  Effects = 400,
  UI = 500,
  Debug = 1000
};

class IRenderBridge
{
public:
  virtual ~IRenderBridge() = default;

  virtual void DrawSprite(const std::string &sprite_id,
    Vector2 position,
    Vector2 scale,
    float rotation,
    Color tint,
    bool flip_x,
    bool flip_y,
    Vector2 origin,
    Vector2 offset,
    Rectangle subregion,
    RenderLayer layer,
    int sub_layer) = 0;

  virtual void DrawAnimation(const core::component::SpriteAnimation &anim,
    Vector2 position,
    Vector2 scale,
    float rotation,
    Color tint,
    bool flip_x,
    bool flip_y,
    Vector2 origin) = 0;

  virtual void LoadSprite(const std::string &filepath) = 0;

  virtual void CameraSetPosition(Vector2 position) = 0;
  virtual void CameraSetMode(CameraMode mode) = 0;
  virtual void CameraSetTarget(Vector2 target) = 0;
  virtual void CameraSetBounds(Rectangle bounds) = 0;
  virtual void CameraSetZoom(float zoom) = 0;
  virtual void CameraSetSmoothness(float smoothness) = 0;
  virtual void CameraUpdate(float delta_time) = 0;
  virtual void CameraSetDeadzone(Vector2 size) = 0;

  virtual const Rectangle GetActiveCameraBounds() const = 0;
  virtual std::vector<uint8_t> GetActivePasses() const = 0;
};

void SetRenderBridge(std::shared_ptr<IRenderBridge> bridge);
std::shared_ptr<IRenderBridge> GetRenderBridge();
}// namespace chroma::shared::render
