#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Item.h"
#include "chroma/shared/core/components/Weapon.h"

namespace chroma::shared::core::component {
Inventory::Inventory(int capacity) : capacity_(capacity) { type_ = ComponentType::INVENTORY; }

Inventory::Inventory() : capacity_(0) { type_ = ComponentType::INVENTORY; }

bool Inventory::AddInventory(const std::shared_ptr<Component> &item)
{
  const size_t used = items_.size() + weapons_.size();
  if (std::cmp_greater_equal(used, static_cast<size_t>(capacity_))) { return false; }

  if (item->IsType(ComponentType::ITEM)) {
    items_.emplace_back(std::static_pointer_cast<Item>(item));
    return true;
  }

  if (item->IsType(ComponentType::WEAPON)) {
    weapons_.emplace_back(std::static_pointer_cast<Weapon>(item));
    return true;
  }

  return false;
}

bool Inventory::RemoveInventory(const std::shared_ptr<Component> &item)
{
  if (item->IsType(ComponentType::ITEM)) {
    auto it = std::ranges::find(items_, std::static_pointer_cast<Item>(item));
    if (it != items_.end()) {
      items_.erase(it);
      return true;
    }
  } else if (item->IsType(ComponentType::WEAPON)) {
    auto it = std::ranges::find(weapons_, std::static_pointer_cast<Weapon>(item));
    if (it != weapons_.end()) {
      weapons_.erase(it);
      return true;
    }
  }
  return false;
}

const std::vector<std::shared_ptr<Item>> &Inventory::GetItems() const { return items_; }

const std::vector<std::shared_ptr<Weapon>> &Inventory::GetWeapons() const { return weapons_; }

bool Inventory::SetCurrentItem(const std::shared_ptr<Item> &item)
{
  auto it = std::ranges::find(items_, item);
  if (it != items_.end()) {
    current_item_ = item;
    return true;
  }
  return false;
}

bool Inventory::SetCurrentWeapon(const std::shared_ptr<Weapon> &weapon)
{
  auto it = std::ranges::find(weapons_, weapon);
  if (it != weapons_.end()) {
    current_weapon_ = weapon;
    return true;
  }
  return false;
}

const std::shared_ptr<Item> &Inventory::GetCurrentItem() const { return current_item_; }

const std::shared_ptr<Weapon> &Inventory::GetCurrentWeapon() const { return current_weapon_; }

int Inventory::GetCapacity() const { return capacity_; }

void Inventory::SetCapacity(int capacity) { capacity_ = capacity; }

const std::shared_ptr<Weapon> &Inventory::GetWeaponByWeaponType(WeaponType type) const
{
  auto it =
    std::ranges::find_if(weapons_, [type](const std::shared_ptr<Weapon> &w) { return w->GetWeaponType() == type; });

  if (it != weapons_.end()) { return *it; }

  static const std::shared_ptr<Weapon> null_weapon = nullptr;
  return null_weapon;
}


const std::shared_ptr<Weapon> &Inventory::ChangeToNextWeapon()
{
  if (weapons_.empty()) {
    current_weapon_ = nullptr;
    return current_weapon_;
  }

  if (!current_weapon_) {
    current_weapon_ = weapons_.front();
    return current_weapon_;
  }

  auto it = std::ranges::find(weapons_, current_weapon_);

  if (it == weapons_.end()) {
    current_weapon_ = weapons_.front();
    return current_weapon_;
  }

  ++it;
  if (it == weapons_.end()) { it = weapons_.begin(); }

  current_weapon_ = *it;
  return current_weapon_;
}

const std::shared_ptr<Weapon> &Inventory::ChangeToPreviousWeapon()
{
  if (weapons_.empty()) {
    current_weapon_ = nullptr;
    return current_weapon_;
  }

  if (!current_weapon_) {
    current_weapon_ = weapons_.back();
    return current_weapon_;
  }

  auto it = std::ranges::find(weapons_, current_weapon_);

  if (it == weapons_.end()) {
    current_weapon_ = weapons_.back();
    return current_weapon_;
  }

  if (it == weapons_.begin()) { it = weapons_.end(); }
  --it;

  current_weapon_ = *it;
  return current_weapon_;
}

bool Inventory::HasItemType(const WeaponType type) const
{
  auto it = std::ranges::find_if(weapons_, [type](const std::shared_ptr<Weapon> &w) { return w->GetWeaponType() == type; });
  return it != weapons_.end();
}

void Inventory::SetWeaponByWeaponType(const WeaponType type)
{
  auto it = std::ranges::find_if(weapons_, [type](const std::shared_ptr<Weapon> &w) { return w->GetWeaponType() == type; });
  if (it != weapons_.end()) {
    current_weapon_ = *it;
  }
}

}// namespace chroma::shared::core::component