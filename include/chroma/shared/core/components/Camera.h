#pragma once

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/render/RenderBridge.h"

#include <raylib.h>

namespace chroma::shared::core::component {

class CameraComponent : public Component
{
public:
  CameraComponent();
  ~CameraComponent() override = default;

  CameraComponent(const CameraComponent &) = default;
  CameraComponent(CameraComponent &&) = delete;
  CameraComponent &operator=(const CameraComponent &) = default;
  CameraComponent &operator=(CameraComponent &&) = delete;

  void Update(float delta_time) override;

  void SetMode(render::CameraMode mode);
  void SetZoom(float zoom);
  void SetSmoothness(float smoothness);
  void SetBounds(Rectangle bounds);
  void EnableBounds(bool enabled);
  void SetDeadzone(Vector2 size);
  void Setup();

  [[nodiscard]] render::CameraMode GetMode() const { return mode_; }
  [[nodiscard]] float GetZoom() const { return zoom_; }
  [[nodiscard]] float GetSmoothness() const { return smoothness_; }
  [[nodiscard]] bool HasBounds() const { return use_bounds_; }
  [[nodiscard]] Rectangle GetBounds() const { return bounds_; }

private:
  render::CameraMode mode_;
  float zoom_;
  float smoothness_;
  bool use_bounds_;
  Rectangle bounds_;
  Vector2 deadzone_{ 0.0F, 0.0F };
};

}// namespace chroma::shared::core::component
