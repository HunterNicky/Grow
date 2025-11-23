#include "chroma/shared/packet/adapter/ComponentAdapter.h"

#include "chroma/shared/packet/adapter/WeaponAdapter.h"

#include "chroma/shared/core/components/Attack.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/ProjectileType.h"
#include "chroma/shared/core/components/Run.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/Weapon.h"
#include "chroma/shared/factory/WeaponFactory.h"

#include <flatbuffers/flatbuffer_builder.h>
#include <components_generated.h>
#include <memory>

namespace chroma::shared::packet::adapter {

void ComponentAdapter::ToComponent(const std::shared_ptr<core::GameObject> &game_object,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  SpeedToComponent(game_object->GetComponent<core::component::Speed>(), builder, fb_components);
  TransformToComponent(game_object->GetComponent<core::component::Transform>(), builder, fb_components);
  MovementToComponent(game_object->GetComponent<core::component::Movement>(), builder, fb_components);
  ColorToComponent(game_object->GetComponent<core::component::Coloring>(), builder, fb_components);
  HealthToComponent(game_object->GetComponent<core::component::Health>(), builder, fb_components);
  RunToComponent(game_object->GetComponent<core::component::Run>(), builder, fb_components);
  InventoryToComponent(game_object->GetComponent<core::component::Inventory>(), builder, fb_components);
  AttackToComponent(game_object->GetComponent<core::component::Attack>(), builder, fb_components);
  ProjectileTypeToComponent(game_object->GetComponent<core::component::ProjectileType>(), builder, fb_components);
}

void ComponentAdapter::FromComponent(const Game::Component &component, std::shared_ptr<core::GameObject> &game_object)
{
  switch (component.type_type()) {
  case Game::ComponentUnion::Velocity:
    ComponentToSpeed(&component, game_object);
    break;
  case Game::ComponentUnion::Position:
    ComponentToTransform(&component, game_object);
    break;
  case Game::ComponentUnion::Movement:
    ComponentToMovement(&component, game_object);
    break;
  case Game::ComponentUnion::Color:
    ComponentToColor(&component, game_object);
    break;
  case Game::ComponentUnion::Health:
    ComponentToHealth(&component, game_object);
    break;
  case Game::ComponentUnion::Run:
    ComponentToRun(&component, game_object);
    break;
  case Game::ComponentUnion::Inventory:
    ComponentToInventory(&component, game_object);
    break;
  case Game::ComponentUnion::Attack:
    ComponentToAttack(&component, game_object);
    break;
  case Game::ComponentUnion::ProjectileType:
    ComponentToProjectileType(&component, game_object);
    break;
  default:
    return;
  }
}

void ComponentAdapter::SpeedToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto speed = std::static_pointer_cast<core::component::Speed>(component);
  const auto vel = speed->GetSpeed();
  const auto fb_vel = Game::CreateVelocity(builder, Game::CreateVec2(builder, vel.x, vel.y));
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Velocity, fb_vel.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::TransformToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto transform = std::static_pointer_cast<core::component::Transform>(component);
  const auto pos = transform->GetPosition();
  const auto fb_pos = Game::CreatePosition(builder, Game::CreateVec2(builder, pos.x, pos.y));
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Position, fb_pos.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::MovementToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto movement = std::static_pointer_cast<core::component::Movement>(component);
  const auto dir = movement->GetDirection();
  const auto fb_mov = Game::CreateMovement(builder, Game::CreateVec2(builder, dir.x, dir.y));
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Movement, fb_mov.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::ColorToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto color = std::static_pointer_cast<core::component::Coloring>(component);
  const auto fb_color = Game::CreateColor(builder,
    static_cast<int8_t>(color->GetRed()),
    static_cast<int8_t>(color->GetGreen()),
    static_cast<int8_t>(color->GetBlue()),
    static_cast<int8_t>(color->GetAlpha()));
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Color, fb_color.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::HealthToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto health = std::static_pointer_cast<core::component::Health>(component);
  const auto fb_health = Game::CreateHealth(builder, *health->GetCurrentHealth(), *health->GetMaxHealth());
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Health, fb_health.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::ComponentToRun(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_run = component->type_as_Run();
  if (fb_run == nullptr) { return; }

  auto run = game_object->GetComponent<core::component::Run>();
  if (!run) {
    auto new_run = std::make_shared<core::component::Run>();
    game_object->AttachComponent(new_run);
    run = new_run;
  }

  run->SetRunning(fb_run->running());
  run->SetSpeedFactor(fb_run->factor_speed());
}

void ComponentAdapter::ComponentToTransform(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_pos = component->type_as_Position();
  if (fb_pos == nullptr) { return; }

  auto transform = game_object->GetComponent<core::component::Transform>();
  if (!transform) {
    auto new_transform = std::make_shared<core::component::Transform>();
    new_transform->SetScale(Vector2{ 1.0F, 1.0F });
    game_object->AttachComponent(new_transform);
    transform = new_transform;
  }

  const auto *pos_vec = fb_pos->pos();
  transform->SetPosition(Vector2{ pos_vec->x(), pos_vec->y() });
}


void ComponentAdapter::ComponentToSpeed(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_vel = component->type_as_Velocity();
  if (fb_vel == nullptr) { return; }

  auto speed = game_object->GetComponent<core::component::Speed>();
  if (!speed) {
    auto new_speed = std::make_shared<core::component::Speed>();
    game_object->AttachComponent(new_speed);
    speed = new_speed;
  }

  const auto *v = fb_vel->vel();
  speed->SetSpeed(Vector2{ v->x(), v->y() });
}


void ComponentAdapter::ComponentToMovement(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_mov = component->type_as_Movement();
  if (fb_mov == nullptr) { return; }

  auto movement = game_object->GetComponent<core::component::Movement>();
  if (!movement) {
    auto new_movement = std::make_shared<core::component::Movement>();
    game_object->AttachComponent(new_movement);
    movement = new_movement;
  }

  const auto *v = fb_mov->mov();
  movement->SetDirection(Vector2{ v->x(), v->y() });
}

void ComponentAdapter::ComponentToColor(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_color = component->type_as_Color();
  if (fb_color == nullptr) { return; }

  auto color = game_object->GetComponent<core::component::Coloring>();
  if (!color) {
    auto new_color = std::make_shared<core::component::Coloring>();
    game_object->AttachComponent(new_color);
    color = new_color;
  }

  color->SetColoring(fb_color->r(), fb_color->g(), fb_color->b(), fb_color->a());
}


void ComponentAdapter::ComponentToHealth(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_health = component->type_as_Health();
  if (fb_health == nullptr) { return; }

  auto health = game_object->GetComponent<core::component::Health>();
  if (!health) {
    auto new_health = std::make_shared<core::component::Health>();
    game_object->AttachComponent(new_health);
    health = new_health;
  }

  health->SetCurrentHealth(fb_health->current_health());
  health->SetMaxHealth(fb_health->max_health());
}

void ComponentAdapter::ComponentToInventory(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_inventory = component->type_as_Inventory();
  if (fb_inventory == nullptr) { return; }

  auto inventory = game_object->GetComponent<core::component::Inventory>();
  if (!inventory) {
    auto new_inventory = std::make_shared<core::component::Inventory>();
    game_object->AttachComponent(new_inventory);
    inventory = new_inventory;
  }
  inventory->SetCapacity(fb_inventory->capacity());

  const auto *fb_weapons = fb_inventory->weapons();
  if (fb_weapons != nullptr) {
    for (const auto &fb_weapon : *fb_weapons) {

      auto type = static_cast<core::component::WeaponType>(fb_weapon->weapon_type());
      auto weapon = inventory->GetWeaponByWeaponType(type);
      if (!weapon) {
        weapon = factory::WeaponFactory::CreateWeaponByType(type);
        inventory->AddInventory(weapon);
      }
      WeaponAdapter::FromComponent(*fb_weapon, weapon);
    }
  }

  const auto *fb_items = fb_inventory->itens();
  if (fb_items != nullptr) {
    for (const auto &fb_item : *fb_items) { ComponentToItem(fb_item, inventory); }
  }

  const auto *current_weapon = fb_inventory->current_weapon();
  if (current_weapon != nullptr) {
    auto type = static_cast<core::component::WeaponType>(current_weapon->weapon_type());
    auto weapon = inventory->GetWeaponByWeaponType(type);
    if (weapon) { inventory->SetCurrentWeapon(weapon); }
  }

  const auto *current_item = fb_inventory->current_item();
  if (current_item != nullptr) {
    auto it = std::make_shared<core::component::Item>(
      current_item->name()->str(), current_item->description()->str(), current_item->weight());
    inventory->SetCurrentItem(it);
  }
}

void ComponentAdapter::ComponentToItem(const Game::Item *item,
  const std::shared_ptr<core::component::Inventory> &inventory)
{
  if (item == nullptr) { return; }

  const auto it =
    std::make_shared<core::component::Item>(item->name()->str(), item->description()->str(), item->weight());

  inventory->AddInventory(it);
}

void ComponentAdapter::AttackToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto attack = std::static_pointer_cast<core::component::Attack>(component);
  const auto fb_attack = Game::CreateAttack(builder, attack->IsAttacking());
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Attack, fb_attack.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::RunToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto run = std::static_pointer_cast<core::component::Run>(component);
  const auto fb_run = Game::CreateRun(builder, run->IsRunning(), run->GetSpeedFactor());
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Run, fb_run.Union());
  fb_components.push_back(fb_component);
}

void ComponentAdapter::InventoryToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto inventory = std::static_pointer_cast<core::component::Inventory>(component);

  std::vector<flatbuffers::Offset<Game::Weapon>> weapons_offsets;
  for (const auto &weapon : inventory->GetWeapons()) { WeaponAdapter::ToComponent(weapon, builder, weapons_offsets); }

  std::vector<flatbuffers::Offset<Game::Item>> items_offsets;
  for (const auto &item : inventory->GetItems()) { ItemToComponent(item, builder, items_offsets); }
  auto fb_items_vec = builder.CreateVector(items_offsets);

  auto fb_weapons_vec = builder.CreateVector(weapons_offsets);

  auto current_weapon = inventory->GetCurrentWeapon();
  flatbuffers::Offset<Game::Weapon> fb_current_weapon = 0;

  if (current_weapon) {
    std::vector<flatbuffers::Offset<Game::Weapon>> tmp;
    WeaponAdapter::ToComponent(current_weapon, builder, tmp);

    if (!tmp.empty()) { fb_current_weapon = tmp[0]; }
  }

  auto fb_inventory =
    Game::CreateInventory(builder, inventory->GetCapacity(), fb_weapons_vec, fb_items_vec, fb_current_weapon);

  auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Inventory, fb_inventory.Union());

  fb_components.push_back(fb_component);
}

void ComponentAdapter::ItemToComponent(const std::shared_ptr<core::component::Item> &item,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Item>> &fb_items)
{
  if (!item) { return; }

  auto fb_item = Game::CreateItem(
    builder, builder.CreateString(item->GetName()), builder.CreateString(item->GetDescription()), item->GetWeight());

  fb_items.push_back(fb_item);
}

void ComponentAdapter::ComponentToAttack(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_attack = component->type_as_Attack();
  if (fb_attack == nullptr) { return; }

  auto attack = game_object->GetComponent<core::component::Attack>();
  if (!attack) {
    auto new_attack = std::make_shared<core::component::Attack>(false);
    game_object->AttachComponent(new_attack);
    attack = new_attack;
  }

  attack->SetAttacking(fb_attack->attack());
}

void ComponentAdapter::ComponentToProjectileType(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_proj_type = component->type_as_ProjectileType();
  if (fb_proj_type == nullptr) { return; }

  auto projectile_type = game_object->GetComponent<core::component::ProjectileType>();
  if (!projectile_type) {
    auto new_proj_type = std::make_shared<core::component::ProjectileType>();
    game_object->AttachComponent(new_proj_type);
    projectile_type = new_proj_type;
  }

  projectile_type->SetProjectileType(static_cast<core::component::TypeProjectile>(fb_proj_type->proj_type_()));
}

void ComponentAdapter::ProjectileTypeToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
  if (!component) { return; }
  const auto projectile_type = std::static_pointer_cast<core::component::ProjectileType>(component);
  const auto fb_proj_type =
    Game::CreateProjectileType(builder, static_cast<Game::TypeProjectile>(projectile_type->GetProjectileType()));
  const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::ProjectileType, fb_proj_type.Union());
  fb_components.push_back(fb_component);
}

}// namespace chroma::shared::packet::adapter