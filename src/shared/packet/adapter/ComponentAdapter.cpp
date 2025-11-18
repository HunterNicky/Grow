#include "chroma/shared/packet/adapter/ComponentAdapter.h"

#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Run.h"
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
    HealthToComponent( game_object->GetComponent<core::component::Health>(), builder, fb_components);
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
        health->GetCurrentHealth(),
        health->GetMaxHealth());
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

} // namespace chroma::shared::packet::adapter