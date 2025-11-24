#pragma once

#include <memory>
#include <raylib.h>
#include <shared_mutex>
#include <vector>

#include "chroma/shared/core/components/ColliderBox.h"

namespace chroma::shared::collision {

class Quadtree
{
public:
  Quadtree(int level, Rectangle bounds);
  ~Quadtree() = default;

  void Clear();
  void Insert(const std::shared_ptr<core::component::ColliderBox> &collider_box);
  void Retrieve(std::vector<std::shared_ptr<core::component::ColliderBox>> &return_objects,
    const std::shared_ptr<core::component::ColliderBox> &collider_box) const;

private:
  static constexpr int max_objects = 10;

  int level_;
  Rectangle bounds_;
  std::vector<std::shared_ptr<core::component::ColliderBox>> collider_boxes_;

  std::unique_ptr<Quadtree> northwest_;
  std::unique_ptr<Quadtree> northeast_;
  std::unique_ptr<Quadtree> southwest_;
  std::unique_ptr<Quadtree> southeast_;

  mutable std::shared_mutex mutex_;

  void Split();
  [[nodiscard]] int GetIndex(const Rectangle &rect) const;
  static Rectangle GetObjectBounds(const std::shared_ptr<core::component::ColliderBox> &collider_box);
};

}// namespace chroma::shared::collision