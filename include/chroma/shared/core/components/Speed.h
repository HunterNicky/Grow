#pragma once

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>

namespace chroma::shared::core::component {
class Speed : public Component
{
public:
  Speed();
  Speed(const Speed &) = default;
  Speed(Speed &&) = delete;
  Speed &operator=(const Speed &) = default;
  Speed &operator=(Speed &&) = delete;

  explicit Speed(float speed);
  explicit Speed(Vector2 speed);
  ~Speed() override = default;
  [[nodiscard]] Vector2 GetSpeed() const;
  void SetSpeed(Vector2 new_speed);

private:
  Vector2 speed_;
};
}// namespace chroma::shared::core::component
