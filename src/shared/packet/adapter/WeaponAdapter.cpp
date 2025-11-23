#include "chroma/shared/packet/adapter/WeaponAdapter.h"
#include "chroma/shared/core/components/Weapon.h"

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/vector.h>
#include <components_generated.h>
#include <common_generated.h>
#include <memory>
#include <vector>

namespace chroma::shared::packet::adapter {

static Game::WeaponType ConvertWeaponType(core::component::WeaponType t)
{
  switch (t) {
  case core::component::WeaponType::FIST:
    return Game::WeaponType::FIST;
  case core::component::WeaponType::SWORD:
    return Game::WeaponType::SWORD;
  case core::component::WeaponType::BOW:
    return Game::WeaponType::BOW;
  case core::component::WeaponType::AXE:
    return Game::WeaponType::AXE;
  case core::component::WeaponType::SPEAR:
    return Game::WeaponType::SPEAR;
  case core::component::WeaponType::JAVELIN:
    return Game::WeaponType::JAVELIN;
  default:
    return Game::WeaponType::FIST;
  }
}

void WeaponAdapter::ToComponent(const std::shared_ptr<core::component::Weapon> &weapon,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons)
{
  if (!weapon) { return; }

  switch (weapon->GetWeaponType()) {
  case core::component::WeaponType::SPEAR:
    SpearToWeaponComponent(weapon, builder, fb_weapons);
    break;
  case core::component::WeaponType::JAVELIN:
    JavelinToWeaponComponent(weapon, builder, fb_weapons);
    break;
  case core::component::WeaponType::FIST:
    FistToWeaponComponent(weapon, builder, fb_weapons);
    break;
  default:
    break;
  }
}

void WeaponAdapter::FromComponent(const Game::Weapon &fb_weapon, std::shared_ptr<core::component::Weapon> &weapon)
{
  switch (fb_weapon.weapon_type()) {
  case Game::WeaponType::SPEAR:
    WeaponComponentToSpear(fb_weapon, weapon);
    break;
  case Game::WeaponType::JAVELIN:
    WeaponComponentToJavelin(fb_weapon, weapon);
    break;
  case Game::WeaponType::FIST:
    WeaponComponentToFist(fb_weapon, weapon);
    break;
  default:
    break;
  }
}

void WeaponAdapter::WeaponComponentToSpear(const Game::Weapon &fb_weapon,
  std::shared_ptr<core::component::Weapon> &weapon)
{
  weapon->SetDamage(fb_weapon.damage());
  weapon->SetRange(fb_weapon.range());
  weapon->SetWeight(fb_weapon.weight());
  weapon->SetCooldown(fb_weapon.cooldown());
  weapon->SetLastAttackTime(fb_weapon.last_attack_time());
}

void WeaponAdapter::SpearToWeaponComponent(const std::shared_ptr<core::component::Weapon> &weapon,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons)
{
  if (!weapon) { return; }

  auto fb_weapon = Game::CreateWeapon(builder,
    ConvertWeaponType(weapon->GetWeaponType()),
    weapon->GetDamage(),
    weapon->GetRange(),
    weapon->GetWeight(),
    weapon->GetCooldown(),
    weapon->GetLastAttackTime());

  fb_weapons.push_back(fb_weapon);
}

void WeaponAdapter::WeaponComponentToJavelin(const Game::Weapon &fb_weapon,
  std::shared_ptr<core::component::Weapon> &weapon)
{
  weapon->SetDamage(fb_weapon.damage());
  weapon->SetRange(fb_weapon.range());
  weapon->SetWeight(fb_weapon.weight());
  weapon->SetCooldown(fb_weapon.cooldown());
  weapon->SetLastAttackTime(fb_weapon.last_attack_time());
}

void WeaponAdapter::JavelinToWeaponComponent(const std::shared_ptr<core::component::Weapon> &weapon,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons)
{
  if (!weapon) { return; }

  auto fb_weapon = Game::CreateWeapon(builder,
    ConvertWeaponType(weapon->GetWeaponType()),
    weapon->GetDamage(),
    weapon->GetRange(),
    weapon->GetWeight(),
    weapon->GetCooldown(),
    weapon->GetLastAttackTime());

  fb_weapons.push_back(fb_weapon);
}

void WeaponAdapter::WeaponComponentToFist(const Game::Weapon &fb_weapon,
  std::shared_ptr<core::component::Weapon> &weapon)
{
  weapon->SetDamage(fb_weapon.damage());
  weapon->SetRange(fb_weapon.range());
  weapon->SetWeight(fb_weapon.weight());
  weapon->SetCooldown(fb_weapon.cooldown());
  weapon->SetLastAttackTime(fb_weapon.last_attack_time());
}

void WeaponAdapter::FistToWeaponComponent(const std::shared_ptr<core::component::Weapon> &weapon,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons)
{
  if (!weapon) { return; }

  auto fb_weapon = Game::CreateWeapon(builder,
    ConvertWeaponType(weapon->GetWeaponType()),
    weapon->GetDamage(),
    weapon->GetRange(),
    weapon->GetWeight(),
    weapon->GetCooldown(),
    weapon->GetLastAttackTime());

  fb_weapons.push_back(fb_weapon);
}
}// namespace chroma::shared::packet::adapter