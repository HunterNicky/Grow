#pragma once

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Item.h"
#include "chroma/shared/core/components/Weapon.h"

#include <memory>
#include <vector>

namespace chroma::shared::core::component {

class Inventory : public Component
{
public:
  explicit Inventory(int capacity);
  Inventory();
  ~Inventory() override = default;

  Inventory(const Inventory &) = delete;
  Inventory &operator=(const Inventory &) = delete;
  Inventory(Inventory &&) noexcept = default;
  Inventory &operator=(Inventory &&) noexcept = default;

  [[nodiscard]] int GetCapacity() const;
  void SetCapacity(int capacity);

  bool AddInventory(const std::shared_ptr<Component> &item);
  bool RemoveInventory(const std::shared_ptr<Component> &item);

  bool SetCurrentItem(const std::shared_ptr<Item> &item);
  bool SetCurrentWeapon(const std::shared_ptr<Weapon> &weapon);

  [[nodiscard]] const std::vector<std::shared_ptr<Item>> &GetItems() const;
  [[nodiscard]] const std::vector<std::shared_ptr<Weapon>> &GetWeapons() const;

  [[nodiscard]] const std::shared_ptr<Item> &GetCurrentItem() const;
  [[nodiscard]] const std::shared_ptr<Weapon> &GetCurrentWeapon() const;

  [[nodiscard]] const std::shared_ptr<Weapon> &GetWeaponByWeaponType(WeaponType type) const;

  const std::shared_ptr<Weapon> &ChangeToNextWeapon();
  const std::shared_ptr<Weapon> &ChangeToPreviousWeapon();

private:
  int capacity_;
  std::vector<std::shared_ptr<Item>> items_;
  std::vector<std::shared_ptr<Weapon>> weapons_;

  std::shared_ptr<Item> current_item_;
  std::shared_ptr<Weapon> current_weapon_;
};

}// namespace chroma::shared::core::component