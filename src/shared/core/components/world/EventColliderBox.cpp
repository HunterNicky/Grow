#include "chroma/shared/core/components/world/EventColliderBox.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Transform.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::core::component {

EventColliderBox::EventColliderBox()
  : size_{ 32.0F, 32.0F }, offset_{ 0.0F, 0.0F }, world_position_{ 0.0F, 0.0F },
    bounding_box_(std::make_shared<Rectangle>(0.0F, 0.0F, 0.0F, 0.0F))
{
  type_ = ComponentType::EVENT_COLLIDER_BOX;
}

EventColliderBox::~EventColliderBox() = default;

EventColliderBox::EventColliderBox(const Vector2 &size, const Vector2 &offset)
  : size_(size), offset_(offset), world_position_{ 0.0F, 0.0F },
    bounding_box_(std::make_shared<Rectangle>(0.0F, 0.0F, size.x, size.y))
{
  type_ = ComponentType::EVENT_COLLIDER_BOX;
}

void EventColliderBox::SetSize(const Vector2 &size) { size_ = size; }

Vector2 EventColliderBox::GetSize() const { return size_; }

void EventColliderBox::SetOffset(const Vector2 &offset) { offset_ = offset; }

Vector2 EventColliderBox::GetOffset() const { return offset_; }

Vector2 EventColliderBox::GetWorldPosition() const { return world_position_; }

std::shared_ptr<Rectangle> EventColliderBox::GetBoundingBox()
{
  bounding_box_->x = world_position_.x;
  bounding_box_->y = world_position_.y;
  bounding_box_->width = size_.x;
  bounding_box_->height = size_.y;

  return bounding_box_;
}

void EventColliderBox::Update([[maybe_unused]] float delta_time)
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

void EventColliderBox::Render() { DrawRectangleLinesEx(*GetBoundingBox(), 1.0F, YELLOW); }

}// namespace chroma::shared::core::component
