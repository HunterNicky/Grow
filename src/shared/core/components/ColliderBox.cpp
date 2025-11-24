#include "chroma/shared/core/components/ColliderBox.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Transform.h"

namespace chroma::shared::core::component {
ColliderBox::ColliderBox() { type_ = ComponentType::COLLIDER_BOX; }

ColliderBox::~ColliderBox() = default;

ColliderBox::ColliderBox(const Vector2 &size, const Vector2 &offset) : size_(size), offset_(offset)
{
  type_ = ComponentType::COLLIDER_BOX;
}

void ColliderBox::SetSize(const Vector2 &size) { size_ = size; }

Vector2 ColliderBox::GetSize() const { return size_; }

void ColliderBox::SetOffset(const Vector2 &offset) { offset_ = offset; }

Vector2 ColliderBox::GetOffset() const { return offset_; }

Vector2 ColliderBox::GetWorldPosition() const { return world_position_; }

Rectangle ColliderBox::GetBoundingBox() const
{
  return Rectangle{ world_position_.x, world_position_.y, size_.x, size_.y };
}

void ColliderBox::Update([[maybe_unused]] float delta_time)
{
  if (const auto owner = game_object_.lock()) {
    if (const auto transform = owner->GetComponent<Transform>()) {
      const Vector2 parent_pos = transform->GetPosition();
      world_position_.x = parent_pos.x + offset_.x;
      world_position_.y = parent_pos.y + offset_.y;
    }
  }
}

void ColliderBox::Render() { DrawRectangleLinesEx(GetBoundingBox(), 1.0F, GREEN); }

}// namespace chroma::shared::core::component