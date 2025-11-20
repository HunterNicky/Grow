#include "chroma/shared/packet/adapter/ComponentAdapter.h"

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Run.h"
#include <components_generated.h>
#include <memory>

namespace chroma::shared::packet::adapter {

static Game::WeaponType ConvertWeaponType(core::component::WeaponType t)
{
    switch (t)
    {
    case core::component::WeaponType::FIST:  return Game::WeaponType::FIST;
    case core::component::WeaponType::SWORD: return Game::WeaponType::SWORD;
    case core::component::WeaponType::BOW:   return Game::WeaponType::BOW;
    case core::component::WeaponType::AXE:   return Game::WeaponType::AXE;
    case core::component::WeaponType::SPEAR: return Game::WeaponType::SPEAR;
    default:
        return Game::WeaponType::FIST;
    }
}

void ComponentAdapter::ToComponent(const std::shared_ptr<core::GameObject> &game_object,
  flatbuffers::FlatBufferBuilder &builder,
  std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    SpeedToComponent(game_object->GetComponent<core::component::Speed>(), builder, fb_components);
    TransformToComponent(game_object->GetComponent<core::component::Transform>(), builder, fb_components);
    MovementToComponent(game_object->GetComponent<core::component::Movement>(), builder, fb_components);
    ColorToComponent(game_object->GetComponent<core::component::Coloring>(), builder, fb_components);   
    HealthToComponent( game_object->GetComponent<core::component::Health>(), builder, fb_components);
    RunToComponent(game_object->GetComponent<core::component::Run>(), builder, fb_components);
    InventoryToComponent(game_object->GetComponent<core::component::Inventory>(), builder, fb_components);
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
    default:
        return;
    }
}

void ComponentAdapter::SpeedToComponent(const std::shared_ptr<core::component::Component> &component,
  flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    if (!component) { return; }
    const auto speed = std::static_pointer_cast<core::component::Speed>(component);
    const auto vel = speed->GetSpeed();
    const auto fb_vel = Game::CreateVelocity(builder,
        Game::CreateVec2(builder, vel.x, vel.y));
    const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Velocity, fb_vel.Union());
    fb_components.push_back(fb_component);
}

void ComponentAdapter::TransformToComponent(const std::shared_ptr<core::component::Component> &component,
    flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    if (!component) { return; }
    const auto transform = std::static_pointer_cast<core::component::Transform>(component);
    const auto pos = transform->GetPosition();
    const auto fb_pos = Game::CreatePosition(builder,
        Game::CreateVec2(builder, pos.x, pos.y));
    const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Position, fb_pos.Union());
    fb_components.push_back(fb_component);
}

void ComponentAdapter::MovementToComponent(const std::shared_ptr<core::component::Component> &component,
    flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    if (!component) { return; }
    const auto movement = std::static_pointer_cast<core::component::Movement>(component);
    const auto dir = movement->GetDirection();
    const auto fb_mov = Game::CreateMovement(builder,
        Game::CreateVec2(builder, dir.x, dir.y));
    const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Movement, fb_mov.Union());
    fb_components.push_back(fb_component);
}

void ComponentAdapter::ColorToComponent(const std::shared_ptr<core::component::Component> &component,
    flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
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
    flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    if (!component) { return; }
    const auto health = std::static_pointer_cast<core::component::Health>(component);
    const auto fb_health = Game::CreateHealth(builder,
        *health->GetCurrentHealth(),
        *health->GetMaxHealth());
    const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Health, fb_health.Union());
    fb_components.push_back(fb_component);
}

void ComponentAdapter::ComponentToRun(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_run = component->type_as_Run();
    if (fb_run == nullptr) { return; }

    const auto run = game_object->GetComponent<core::component::Run>();
    if (!run) { return; }

    run->SetRunning(fb_run->running());
    run->SetSpeedFactor(fb_run->factor_speed());
}

void ComponentAdapter::ComponentToTransform(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_pos = component->type_as_Position();
    if (fb_pos == nullptr) {return; }

    const auto transform = game_object->GetComponent<core::component::Transform>();
    if (!transform ){ return; }

    const auto *pos_vec = fb_pos->pos();
    transform->SetPosition(Vector2{ pos_vec->x(), pos_vec->y() });
}


void ComponentAdapter::ComponentToSpeed(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_vel = component->type_as_Velocity();
    if (fb_vel == nullptr) { return; }

    const auto speed = game_object->GetComponent<core::component::Speed>();
    if (!speed) { return; }

    const auto *v = fb_vel->vel();
    speed->SetSpeed(Vector2{ v->x(), v->y() });
}


void ComponentAdapter::ComponentToMovement(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_mov = component->type_as_Movement();
    if (fb_mov == nullptr) { return; }

    const auto movement = game_object->GetComponent<core::component::Movement>();
    if (!movement) { return; }

    const auto *v = fb_mov->mov();
    movement->SetDirection(Vector2{ v->x(), v->y() });
}

void ComponentAdapter::ComponentToColor(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_color = component->type_as_Color();
    if (fb_color == nullptr) { return; }

    const auto color = game_object->GetComponent<core::component::Coloring>();
    if (!color) { return; }

    color->SetColoring(
        fb_color->r(),
        fb_color->g(),
        fb_color->b(),
        fb_color->a()
    );
}


void ComponentAdapter::ComponentToHealth(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_health = component->type_as_Health();
    if (fb_health == nullptr) { return; }

    const auto health = game_object->GetComponent<core::component::Health>();
    if (!health) { return; }

    health->SetCurrentHealth(fb_health->current_health());
    health->SetMaxHealth(fb_health->max_health());
}

void ComponentAdapter::ComponentToInventory(const Game::Component *component,
    const std::shared_ptr<core::GameObject> &game_object)
{
    const auto *fb_inventory = component->type_as_Inventory();
    if (fb_inventory == nullptr) { return; }

    const auto inventory = game_object->GetComponent<core::component::Inventory>();
    if (!inventory) { return; }

    inventory->SetCapacity(fb_inventory->capacity());

    const auto* fb_weapons = fb_inventory->weapons();
    if (fb_weapons != nullptr) {
        for (const auto& fb_weapon : *fb_weapons) {
            ComponentToWeapon(fb_weapon, inventory);
        }
    }

    const auto* fb_items = fb_inventory->itens();
    if (fb_items != nullptr) {
        for (const auto& fb_item : *fb_items) {
            ComponentToItem(fb_item, inventory);
        }
    }
}

void ComponentAdapter::ComponentToWeapon(const Game::Weapon* weapon,
    const std::shared_ptr<core::component::Inventory>& inventory)
{
    if (weapon == nullptr) { return; }

    const auto w = std::make_shared<core::component::Weapon>(
        static_cast<core::component::WeaponType>(weapon->weapon_type()),
        weapon->damage(),
        weapon->range(),
        weapon->weight(),
        weapon->cooldown()
    );

    inventory->AddInventory(w);
}

void ComponentAdapter::ComponentToItem(const Game::Item* item,
  const std::shared_ptr<core::component::Inventory>& inventory)
{
    if (item == nullptr) { return; }

    const auto it = std::make_shared<core::component::Item>(
        item->name()->str(),
        item->description()->str(),
        item->weight()
    );

    inventory->AddInventory(it);
}

void ComponentAdapter::RunToComponent(const std::shared_ptr<core::component::Component> &component,
    flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    if (!component) { return; }
    const auto run = std::static_pointer_cast<core::component::Run>(component);
    const auto fb_run = Game::CreateRun(builder,
        run->IsRunning(),
        run->GetSpeedFactor());
    const auto fb_component = Game::CreateComponent(builder, Game::ComponentUnion::Run, fb_run.Union());
    fb_components.push_back(fb_component);
}

void ComponentAdapter::InventoryToComponent(
    const std::shared_ptr<core::component::Component> &component,
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<flatbuffers::Offset<Game::Component>> &fb_components)
{
    if (!component) { return; }
    const auto inventory = std::static_pointer_cast<core::component::Inventory>(component);

    std::vector<flatbuffers::Offset<Game::Weapon>> weapons_offsets;
    for (const auto& weapon : inventory->GetWeapons()) {
        WeaponToComponent(weapon, builder, weapons_offsets);
    }

    std::vector<flatbuffers::Offset<Game::Item>> items_offsets;
    for (const auto& item : inventory->GetItems()) {
        ItemToComponent(item, builder, items_offsets);
    }
    auto fb_items_vec = builder.CreateVector(items_offsets);

    auto fb_weapons_vec = builder.CreateVector(weapons_offsets);

    auto fb_inventory = Game::CreateInventory(
        builder,
        inventory->GetCapacity(),
        fb_weapons_vec,
        fb_items_vec
    );

    auto fb_component = Game::CreateComponent(
        builder,
        Game::ComponentUnion::Inventory,
        fb_inventory.Union()
    );

    fb_components.push_back(fb_component);
}


void ComponentAdapter::WeaponToComponent(
    const std::shared_ptr<core::component::Weapon>& weapon,
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<flatbuffers::Offset<Game::Weapon>> &fb_weapons)
{    
    if (!weapon) { return; }

    auto fb_weapon = Game::CreateWeapon(
        builder,
        ConvertWeaponType(weapon->GetWeaponType()),
        weapon->GetDamage(),
        weapon->GetRange(),
        weapon->GetWeight(),
        weapon->GetCooldown()
    );

    fb_weapons.push_back(fb_weapon);
}


void ComponentAdapter::ItemToComponent(const std::shared_ptr<core::component::Item>& item,
  flatbuffers::FlatBufferBuilder &builder, std::vector<flatbuffers::Offset<Game::Item>> &fb_items)
{
    if (!item) { return; }

    auto fb_item = Game::CreateItem(
        builder,
         builder.CreateString(item->GetName()),
        builder.CreateString(item->GetDescription()),
        item->GetWeight()
    );

    fb_items.push_back(fb_item);
}

} // namespace chroma::shared::packet::adapter