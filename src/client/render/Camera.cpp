#include "chroma/client/render/Camera.h"

#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <raymath.h>

namespace chroma::client::render {
Camera::Camera(const float x, const float y, const int virtual_width, const int virtual_height)
  : mode_(CameraMode::Free), target_({ .x = x, .y = y }),
    bounds_({ .x = -INFINITY, .y = -INFINITY, .width = INFINITY, .height = INFINITY }), virtual_width_(virtual_width),
    virtual_height_(virtual_height), smoothness_(5.0F), velocity_({ .x = 0, .y = 0 }), shake_intensity_(0),
    shake_duration_(0), shake_timer_(0)
{
  camera_.target = target_;
  camera_.offset = { .x = static_cast<float>(virtual_width) / 2.0F, .y = static_cast<float>(virtual_height) / 2.0F };
  camera_.rotation = 0.0F;
  camera_.zoom = 1.0F;
}

void Camera::Update(const float delta_time)
{
  auto clamp_to_bounds = [&](Vector2 v) {
    if (std::isinf(bounds_.width) || std::isinf(bounds_.height)) { return v; }
    const float half_w = (static_cast<float>(virtual_width_) / 2.0F) / camera_.zoom;
    const float half_h = (static_cast<float>(virtual_height_) / 2.0F) / camera_.zoom;
    v.x = Clamp(v.x, bounds_.x + half_w, bounds_.x + bounds_.width - half_w);
    v.y = Clamp(v.y, bounds_.y + half_h, bounds_.y + bounds_.height - half_h);
    return v;
  };

  auto apply_deadzone = [&](Vector2 current_center, Vector2 focus) {
    if (deadzone_half_.x <= 0.0F || deadzone_half_.y <= 0.0F) { return focus; }
    Vector2 desired = current_center;
    if (focus.x < current_center.x - deadzone_half_.x) { desired.x = focus.x + deadzone_half_.x; }
    else if (focus.x > current_center.x + deadzone_half_.x) { desired.x = focus.x - deadzone_half_.x; }

    if (focus.y < current_center.y - deadzone_half_.y) { desired.y = focus.y + deadzone_half_.y; }
    else if (focus.y > current_center.y + deadzone_half_.y) { desired.y = focus.y - deadzone_half_.y; }
    return desired;
  };

  switch (mode_) {
  case CameraMode::Free:
    break;

  case CameraMode::Follow: {
    Vector2 const desired = apply_deadzone(camera_.target, target_);
    camera_.target = clamp_to_bounds(desired);
    break;
  }

  case CameraMode::FollowSmooth: {
    Vector2 desired_center = apply_deadzone(camera_.target, target_);
    desired_center = clamp_to_bounds(desired_center);
    const Vector2 diff = Vector2Subtract(desired_center, camera_.target);
    const float dist2 = (diff.x * diff.x) + (diff.y * diff.y);
    const float factor = 1.0F - std::exp(-smoothness_ * delta_time);
    if (dist2 < 0.25F) {
      camera_.target = desired_center;
    } else {
      camera_.target = Vector2Add(camera_.target, Vector2Scale(diff, factor));
    }
    break;
  }

  case CameraMode::Bounded:
    camera_.target = target_;
    ApplyBounds();
    break;
  }

  UpdateShake(delta_time);
}

void Camera::SetTarget(const Vector2 target) { target_ = target; }

void Camera::SetBounds(const Rectangle bounds) { bounds_ = bounds; }

void Camera::SetMode(const CameraMode mode) { mode_ = mode; }

void Camera::SetZoom(const float zoom) { camera_.zoom = Clamp(zoom, 0.1F, 10.0F); }

void Camera::SetSmoothness(const float smoothness) { smoothness_ = smoothness; }

void Camera::SetDeadzone(const float width, const float height)
{
  deadzone_half_.x = std::max(0.0F, width) * 0.5F;
  deadzone_half_.y = std::max(0.0F, height) * 0.5F;
}

Camera2D Camera::GetCamera2D() const { return camera_; }

Vector2 Camera::GetPosition() const { return camera_.target; }

Matrix Camera::GetMatrix() const { return GetCameraMatrix2D(camera_); }

Vector2 Camera::ScreenToWorld(const Vector2 screen_pos) const { return GetScreenToWorld2D(screen_pos, camera_); }

Vector2 Camera::WorldToScreen(const Vector2 world_pos) const { return GetWorldToScreen2D(world_pos, camera_); }

void Camera::Shake(const float intensity, const float duration)
{
  shake_intensity_ = intensity;
  shake_duration_ = duration;
  shake_timer_ = duration;
}

void Camera::UpdateShake(const float delta_time)
{
  if (shake_timer_ <= 0.0F) { return; }

  shake_timer_ -= delta_time;
  const float t = shake_timer_ / shake_duration_;
  const float decay = t * t;
  const float intensity = shake_intensity_ * decay;

  const Vector2 offset = { (static_cast<float>(GetRandomValue(-100, 100)) / 100.0F) * intensity,
    (static_cast<float>(GetRandomValue(-100, 100)) / 100.0F) * intensity };

  camera_.target = Vector2Add(camera_.target, offset);

  if (shake_timer_ <= 0.0F) {
    shake_intensity_ = 0;
    shake_duration_ = 0;
    shake_timer_ = 0;
  }
}

void Camera::ApplyBounds()
{
  if (std::isinf(bounds_.width) || std::isinf(bounds_.height)) { return; }

  const float half_w = (static_cast<float>(virtual_width_) / 2.0F) / camera_.zoom;
  const float half_h = (static_cast<float>(virtual_height_) / 2.0F) / camera_.zoom;

  camera_.target.x = Clamp(camera_.target.x, bounds_.x + half_w, bounds_.x + bounds_.width - half_w);
  camera_.target.y = Clamp(camera_.target.y, bounds_.y + half_h, bounds_.y + bounds_.height - half_h);
}

}// namespace chroma::client::render
