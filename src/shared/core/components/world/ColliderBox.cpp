#include "chroma/shared/core/components/world/ColliderBox.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Transform.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::core::component {
ColliderBox::ColliderBox() : bounding_box_(std::make_shared<Rectangle>(0.0F, 0.0F, 0.0F, 0.0F))
{
  type_ = ComponentType::COLLIDER_BOX;
}

ColliderBox::~ColliderBox() = default;

ColliderBox::ColliderBox(const Vector2 &size, const Vector2 &offset)
  : size_(size), offset_(offset), bounding_box_(std::make_shared<Rectangle>(0.0F, 0.0F, size.x, size.y))
{
  type_ = ComponentType::COLLIDER_BOX;
}

void ColliderBox::SetSize(const Vector2 &size)
{
  size_ = size;
}

Vector2 ColliderBox::GetSize() const { return size_; }

void ColliderBox::SetOffset(const Vector2 &offset)
{
  offset_ = offset;
}

Vector2 ColliderBox::GetOffset() const { return offset_; }

Vector2 ColliderBox::GetWorldPosition() const { return world_position_; }

std::shared_ptr<Rectangle> ColliderBox::GetBoundingBox()
{
  bounding_box_->x = world_position_.x;
  bounding_box_->y = world_position_.y;
  bounding_box_->width = size_.x;
  bounding_box_->height = size_.y;

  return bounding_box_;
}

void ColliderBox::Update([[maybe_unused]] float delta_time)
{
  if (const auto owner = game_object_.lock()) {
    if (const auto transform = owner->GetComponent<Transform>()) {
      const Vector2 parent_pos = transform->GetPosition();
      world_position_.x = parent_pos.x + offset_.x;
      world_position_.y = parent_pos.y + offset_.y;

      bounding_box_->x = world_position_.x;
      bounding_box_->y = world_position_.y;
      bounding_box_->width = size_.x;
      bounding_box_->height = size_.y;
    }
  }
}

void ColliderBox::Render() { DrawRectangleLinesEx(*GetBoundingBox(), 1.0F, GREEN); }

}// namespace chroma::shared::core::component