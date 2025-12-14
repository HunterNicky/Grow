#include "chroma/shared/collision/Quadtree.h"

#include <memory>
#include <mutex>
#include <raylib.h>
#include <shared_mutex>
#include <vector>

namespace chroma::shared::collision {

constexpr int max_levels = 100;

Quadtree::Quadtree(const int level, const Rectangle bounds)
  : level_(level), bounds_(bounds), northwest_(nullptr), northeast_(nullptr), southwest_(nullptr), southeast_(nullptr)
{}

void Quadtree::Clear()
{
  std::unique_lock const lock(mutex_);

  collider_boxes_.clear();

  if (northwest_) {
    northwest_.reset();
    northeast_.reset();
    southwest_.reset();
    southeast_.reset();
  }
}

void Quadtree::Insert(const ColliderEntry &collider_entry)
{
  std::unique_lock const lock(mutex_);

  if (northwest_) {
    const int index = GetIndex(GetObjectBounds(collider_entry));
    if (index != -1) {
      switch (index) {
      case 0:
        northeast_->Insert(collider_entry);
        return;
      case 1:
        northwest_->Insert(collider_entry);
        return;
      case 2:
        southwest_->Insert(collider_entry);
        return;
      case 3:
        southeast_->Insert(collider_entry);
        return;
      default:
        break;
      }
    }
  }

  collider_boxes_.push_back(collider_entry);

  if (collider_boxes_.size() > max_objects && level_ < max_levels) {
    if (!northwest_) { Split(); }

    auto it = collider_boxes_.begin();
    while (it != collider_boxes_.end()) {
      const int index = GetIndex(GetObjectBounds(*it));
      if (index != -1) {
        switch (index) {
        case 0:
          northeast_->Insert(*it);
          break;
        case 1:
          northwest_->Insert(*it);
          break;
        case 2:
          southwest_->Insert(*it);
          break;
        case 3:
          southeast_->Insert(*it);
          break;
        default:
          break;
        }
        it = collider_boxes_.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void Quadtree::Retrieve(std::vector<ColliderEntry> &return_objects, const ColliderEntry &collider_entry) const
{
  std::shared_lock const lock(mutex_);

  return_objects.insert(return_objects.end(), collider_boxes_.begin(), collider_boxes_.end());

  if (!northwest_) { return; }

  const int index = GetIndex(GetObjectBounds(collider_entry));

  if (index != -1) {
    switch (index) {
    case 0:
      northeast_->Retrieve(return_objects, collider_entry);
      break;
    case 1:
      northwest_->Retrieve(return_objects, collider_entry);
      break;
    case 2:
      southwest_->Retrieve(return_objects, collider_entry);
      break;
    case 3:
      southeast_->Retrieve(return_objects, collider_entry);
      break;
    default:
      break;
    }
  } else {
    northeast_->Retrieve(return_objects, collider_entry);
    northwest_->Retrieve(return_objects, collider_entry);
    southwest_->Retrieve(return_objects, collider_entry);
    southeast_->Retrieve(return_objects, collider_entry);
  }
}

void Quadtree::Split()
{
  const float sub_width = bounds_.width / 2.0F;
  const float sub_height = bounds_.height / 2.0F;
  const float x = bounds_.x;
  const float y = bounds_.y;

  northeast_ = std::make_unique<Quadtree>(level_ + 1, Rectangle{ x + sub_width, y, sub_width, sub_height });
  northwest_ = std::make_unique<Quadtree>(level_ + 1, Rectangle{ x, y, sub_width, sub_height });
  southwest_ = std::make_unique<Quadtree>(level_ + 1, Rectangle{ x, y + sub_height, sub_width, sub_height });
  southeast_ =
    std::make_unique<Quadtree>(level_ + 1, Rectangle{ x + sub_width, y + sub_height, sub_width, sub_height });
}

int Quadtree::GetIndex(const Rectangle &rect) const
{
  int index = -1;
  const float vertical_midpoint = bounds_.x + (bounds_.width / 2.0F);
  const float horizontal_midpoint = bounds_.y + (bounds_.height / 2.0F);

  const bool top_quadrant = (rect.y < horizontal_midpoint && rect.y + rect.height < horizontal_midpoint);
  const bool bottom_quadrant = (rect.y > horizontal_midpoint);

  if (rect.x < vertical_midpoint && rect.x + rect.width < vertical_midpoint) {
    if (top_quadrant) {
      index = 1;// NW
    } else if (bottom_quadrant) {
      index = 2;// SW
    }
  } else if (rect.x > vertical_midpoint) {
    if (top_quadrant) {
      index = 0;// NE
    } else if (bottom_quadrant) {
      index = 3;// SE
    }
  }

  return index;
}

Rectangle Quadtree::GetObjectBounds(const ColliderEntry &collider_entry) { return *collider_entry.bounds; }

}// namespace chroma::shared::collision