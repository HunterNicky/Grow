#pragma once

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>

namespace chroma::shared::core::component {

class EventColliderBox final : public Component
{
public:
  EventColliderBox();
  explicit EventColliderBox(const Vector2 &size, const Vector2 &offset = { 0.0F, 0.0F });
  ~EventColliderBox() override;

  void SetSize(const Vector2 &size);
  [[nodiscard]] Vector2 GetSize() const;

  void SetOffset(const Vector2 &offset);
  [[nodiscard]] Vector2 GetOffset() const;

  [[nodiscard]] Vector2 GetWorldPosition() const;
  [[nodiscard]] std::shared_ptr<Rectangle> GetBoundingBox();

  void Update(float delta_time) override;
  void Render() override;

private:
  Vector2 size_{ 32.0F, 32.0F };
  Vector2 offset_{ 0.0F, 0.0F };
  Vector2 world_position_{ 0.0F, 0.0F };
  std::shared_ptr<Rectangle> bounding_box_;
};

}// namespace chroma::shared::core::component
