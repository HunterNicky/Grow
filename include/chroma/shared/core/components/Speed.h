#pragma once

#include "chroma/shared/core/components/Component.h"

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
  ~Speed() override = default;
  [[nodiscard]] float GetSpeed() const;
  void SetSpeed(float new_speed);

private:
  float speed_;
};
}// namespace chroma::shared::core::component
