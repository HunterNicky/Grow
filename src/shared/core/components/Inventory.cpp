#pragma once

#include <algorithm>

#include "chroma/shared/core/components/Inventory.h"

namespace chroma::shared::core::component {
Inventory::Inventory(int capacity)
    : capacity_(capacity) 
    {
    type_ = ComponentType::INVENTORY;
    }

Inventory::Inventory()
: capacity_(0)
{
type_ = ComponentType::INVENTORY;
}

bool Inventory::AddInventory(const std::shared_ptr<Component>& item) {
    if (static_cast<int>(items_.size() + weapons_.size()) >= capacity_) {
        return false;
    }

    if (item->IsType(ComponentType::ITEM)) {
        items_.emplace_back(std::static_pointer_cast<Item>(item));
        return true;
    } 
    else if (item->IsType(ComponentType::WEAPON)) {
        weapons_.emplace_back(std::static_pointer_cast<Weapon>(item));
        return true;
    }
    return false;
}

bool Inventory::RemoveInventory(const std::shared_ptr<Component>& item) {
    if (item->IsType(ComponentType::ITEM)) {
        auto it = std::ranges::find(items_, std::static_pointer_cast<Item>(item));
        if (it != items_.end()) {
            items_.erase(it);
            return true;
        }
    } 
    else if (item->IsType(ComponentType::WEAPON)) {
        auto it = std::ranges::find(weapons_, std::static_pointer_cast<Weapon>(item));
        if (it != weapons_.end()) {
            weapons_.erase(it);
            return true;
        }
    }
    return false;
}

const std::vector<std::shared_ptr<Item>>& Inventory::GetItems() const {
    return items_;
}

const std::vector<std::shared_ptr<Weapon>>& Inventory::GetWeapons() const {
    return weapons_;
}

bool Inventory::SetCurrentItem(const std::shared_ptr<Item>& item) {
    auto it = std::ranges::find(items_, item);
    if (it != items_.end()) {
        current_item_ = item;
        return true;
    }
    return false;
}

bool Inventory::SetCurrentWeapon(const std::shared_ptr<Weapon>& weapon) {
    auto it = std::ranges::find(weapons_, weapon);
    if (it != weapons_.end()) {
        current_weapon_ = weapon;
        return true;
    }
    return false;
}

const std::shared_ptr<Item>& Inventory::GetCurrentItem() const {
    return current_item_;
}

const std::shared_ptr<Weapon>& Inventory::GetCurrentWeapon() const {
    return current_weapon_;
}

} // namespace chroma::shared::core::component