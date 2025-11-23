#pragma once

#include "chroma/shared/core/components/Weapon.h"

#include <components_generated.h>

namespace chroma::shared::packet::adapter {
class WeaponAdapter
{
public:
  static void ToComponent(const std::shared_ptr<core::component::Weapon> &weapon,
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons);

  static void FromComponent(const Game::Weapon &fb_weapon, std::shared_ptr<core::component::Weapon> &weapon);

private:
  static void WeaponComponentToSpear(const Game::Weapon &fb_weapon, std::shared_ptr<core::component::Weapon> &weapon);
  static void WeaponComponentToFist(const Game::Weapon &fb_weapon, std::shared_ptr<core::component::Weapon> &weapon);
  static void WeaponComponentToJavelin(const Game::Weapon &fb_weapon, std::shared_ptr<core::component::Weapon> &weapon);

  static void SpearToWeaponComponent(const std::shared_ptr<core::component::Weapon> &weapon,
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons);
  static void JavelinToWeaponComponent(const std::shared_ptr<core::component::Weapon> &weapon,
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons);
  static void FistToWeaponComponent(const std::shared_ptr<core::component::Weapon> &weapon,
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons);
};
}// namespace chroma::shared::packet::adapter