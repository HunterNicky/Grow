#include "chroma/shared/core/components/Camera.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/render/RenderBridge.h"

namespace chroma::shared::core::component {

CameraComponent::CameraComponent()
  : mode_(render::CameraMode::FollowSmooth), zoom_(1.0F), smoothness_(5.0F), use_bounds_(false), bounds_{}
{
  type_ = ComponentType::CAMERA;
}

void CameraComponent::Setup()
{
  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }
  const auto obj = GetGameObject();
  if (!obj) { return; }
  const auto transform = obj->GetComponent<Transform>();
  if (transform) { bridge->CameraSetPosition(transform->GetPosition()); }
}

void CameraComponent::Update(const float delta_time)
{
  // if (!IsAutonomousProxy()) { return; }
  if (HasAuthority()) { return; }

  const auto bridge = render::GetRenderBridge();
  const auto obj = GetGameObject();
  if (!bridge || !obj) { return; }

  const auto transform = obj->GetComponent<Transform>();
  if (!transform) { return; }

  bridge->CameraSetMode(mode_);
  bridge->CameraSetZoom(zoom_);
  bridge->CameraSetSmoothness(smoothness_);
  if (use_bounds_) { bridge->CameraSetBounds(bounds_); }
  if (deadzone_.x > 0.0F && deadzone_.y > 0.0F) { bridge->CameraSetDeadzone(deadzone_); }
  bridge->CameraSetTarget(transform->GetPosition());
  bridge->CameraUpdate(delta_time);
}

void CameraComponent::SetMode(const render::CameraMode mode) { mode_ = mode; }

void CameraComponent::SetZoom(const float zoom) { zoom_ = zoom; }

void CameraComponent::SetSmoothness(const float smoothness) { smoothness_ = smoothness; }

void CameraComponent::SetBounds(const Rectangle bounds)
{
  bounds_ = bounds;
  use_bounds_ = true;
}

void CameraComponent::EnableBounds(const bool enabled) { use_bounds_ = enabled; }

void CameraComponent::SetDeadzone(const Vector2 size) { deadzone_ = size; }

}// namespace chroma::shared::core::component
