#pragma once

#include <raylib.h>

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
class Transform : public Component
{
public:
  Transform();
  Transform(const Transform &) = default;
  Transform(Transform &&) = delete;
  Transform &operator=(const Transform &) = default;
  Transform &operator=(Transform &&) = delete;
  ~Transform() override = default;

  void Translate(const Vector2 &translation);
  void Translate(float x, float y);

  void Scale(const Vector2 &scale);
  void Scale(float x, float y);

  void Rotate(float angle);

  void SetPosition(const Vector2 &position);
  void SetPosition(float x, float y);

  void SetScale(const Vector2 &scale);
  void SetScale(float x, float y);

  void SetRotation(float angle);

  [[nodiscard]] Vector2 GetPosition() const;
  [[nodiscard]] Vector2 GetScale() const;
  [[nodiscard]] float GetRotation() const;

private:
  Vector2 position_;
  Vector2 scale_;
  float rotation_;
};
}// namespace chroma::shared::core::component
