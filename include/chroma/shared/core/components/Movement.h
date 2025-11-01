#pragma once

#include "chroma/shared/core/components/Component.h"
#include <raylib.h>

namespace chroma::shared::core::component {
class Movement : public Component
{
public:
  Movement();
  Movement(const Movement &) = default;
  Movement(Movement &&) = delete;
  Movement &operator=(const Movement &) = default;
  Movement &operator=(Movement &&) = delete;

  explicit Movement(Vector2 direction);
  ~Movement() override = default;

  [[nodiscard]] Vector2 GetDirection() const;
  void SetDirection(Vector2 new_direction);
  void Reset();

private:
  Vector2 direction_;
};
}// namespace chroma::shared::core::component
