#include <raylib.h>

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Transform.h"

namespace chroma::shared::core::component {
Transform::Transform() : position_({ .x = 0, .y = 0 }), scale_({ .x = 0, .y = 0 }), rotation_(0.0F)
{
  type_ = ComponentType::TRANSFORM;
}

void Transform::Translate(const float x, const float y)
{
  position_.x += x;
  position_.y += y;
}

void Transform::Translate(const Vector2 &translation) { Translate(translation.x, translation.y); }

void Transform::Scale(const float x, const float y)
{
  scale_.x += x;
  scale_.y += y;
}

void Transform::Scale(const Vector2 &scale) { Scale(scale.x, scale.y); }

void Transform::Rotate(const float angle) { rotation_ += angle; }

void Transform::SetPosition(const float x, const float y)
{
  position_.x = x;
  position_.y = y;
}

void Transform::SetPosition(const Vector2 &position) { SetPosition(position.x, position.y); }

void Transform::SetScale(const float x, const float y)
{
  scale_.x = x;
  scale_.y = y;
}

void Transform::SetScale(const Vector2 &scale) { SetScale(scale.x, scale.y); }

void Transform::SetRotation(const float angle) { rotation_ = angle; }

Vector2 Transform::GetPosition() const { return position_; }

Vector2 Transform::GetScale() const { return scale_; }

float Transform::GetRotation() const { return rotation_; }
}// namespace chroma::shared::core::component
