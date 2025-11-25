#pragma once

#include "chroma/shared/core/components/Component.h"

#include <memory>
#include <raylib.h>
#include <shared_mutex>
#include <uuid_v4.h>
#include <vector>

namespace chroma::shared::collision {

struct ColliderEntry
{
  UUIDv4::UUID id;
  std::shared_ptr<Rectangle> bounds;
  std::shared_ptr<core::component::Component> component;

  bool operator==(const ColliderEntry &other) const
  {
    return other.id == id && other.bounds->x == bounds->x && other.bounds->y == bounds->y
        && other.bounds->width == bounds->width && other.bounds->height == bounds->height
        && other.component == component;
  }
};

class Quadtree
{
public:
  Quadtree(int level, Rectangle bounds);
  ~Quadtree() = default;

  void Clear();
  void Insert(const ColliderEntry &collider_entry);
  void Retrieve(std::vector<ColliderEntry> &return_objects, const ColliderEntry &collider_entry) const;

private:
  static constexpr int max_objects = 10;

  int level_;
  Rectangle bounds_;
  std::vector<ColliderEntry> collider_boxes_;

  std::unique_ptr<Quadtree> northwest_;
  std::unique_ptr<Quadtree> northeast_;
  std::unique_ptr<Quadtree> southwest_;
  std::unique_ptr<Quadtree> southeast_;

  mutable std::shared_mutex mutex_;

  void Split();
  [[nodiscard]] int GetIndex(const Rectangle &rect) const;
  static Rectangle GetObjectBounds(const ColliderEntry &collider_entry);
};

}// namespace chroma::shared::collision