#pragma once
#include <cstdint>
#include <raylib.h>

namespace chroma::client::render {
enum class CameraMode : uint8_t { Free = 0, Follow = 1, FollowSmooth = 2, Bounded = 3 };

class Camera
{
public:
  Camera(float x, float y, int virtual_width, int virtual_height);

  void Update(float delta_time);
  void SetTarget(Vector2 target);
  void SetBounds(Rectangle bounds);
  void SetMode(CameraMode mode);
  void SetZoom(float zoom);
  void SetSmoothness(float smoothness);
  void SetDeadzone(float width, float height);

  [[nodiscard]] Camera2D GetCamera2D() const;
  [[nodiscard]] Vector2 GetPosition() const;
  [[nodiscard]] Matrix GetMatrix() const;
  [[nodiscard]] Rectangle GetBounds() const;

  [[nodiscard]] Vector2 ScreenToWorld(Vector2 screen_pos) const;
  [[nodiscard]] Vector2 WorldToScreen(Vector2 world_pos) const;

  void Shake(float intensity, float duration);

private:
  Camera2D camera_{};
  CameraMode mode_{};
  Vector2 target_{};
  Rectangle bounds_{};
  int virtual_width_{}, virtual_height_{};

  float smoothness_{};
  Vector2 velocity_{};

  // Deadzone half-size in world units; if zero, disabled
  Vector2 deadzone_half_{ 0.0F, 0.0F };

  float shake_intensity_{};
  float shake_duration_{};
  float shake_timer_{};

  void UpdateShake(float delta_time);
  void ApplyBounds();
};
}// namespace chroma::client::render
