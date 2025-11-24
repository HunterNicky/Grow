#pragma once

#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Attack.h"
#include "chroma/shared/core/components/AudioListener.h"
#include "chroma/shared/core/components/Camera.h"
#include "chroma/shared/core/components/ColliderBox.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/ProjectileType.h"
#include "chroma/shared/core/components/Run.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Transform.h"

#include <memory>
#include <raylib.h>

namespace chroma::shared::builder {

template<typename T> class GameObjectBuilder
{
  static_assert(std::is_base_of_v<core::GameObject, T>, "T must derive from GameObject");

private:
  std::shared_ptr<T> obj_;

public:
  GameObjectBuilder() : obj_(std::make_shared<T>()) {};

  // ------------------------------------------
  //   GAMEOBJECT FIELDS
  // ------------------------------------------

  GameObjectBuilder &Id(const UUIDv4::UUID &uuid)
  {
    obj_->SetId(uuid);
    return *this;
  }

  GameObjectBuilder &Type(core::GameObjectType type)
  {
    obj_->SetType(type);
    return *this;
  }

  GameObjectBuilder &Layer(uint32_t layer)
  {
    obj_->SetLayer(layer);
    return *this;
  }

  GameObjectBuilder &Active(bool active)
  {
    obj_->SetActive(active);
    return *this;
  }

  GameObjectBuilder &NetRole(core::NetRole role)
  {
    obj_->SetNetRole(role);
    return *this;
  }

  // ------------------------------------------
  //   COMPONENTES
  // ------------------------------------------

  GameObjectBuilder &AddTransform(const Vector2 pos, const Vector2 scale = { 1.0F, 1.0F })
  {
    auto transform = std::make_shared<core::component::Transform>();
    transform->SetPosition(pos);
    transform->SetScale(scale);
    obj_->AttachComponent(transform);
    return *this;
  }

  GameObjectBuilder &AddSpeed(float speed)
  {
    auto speed_component = std::make_shared<core::component::Speed>(speed);
    obj_->AttachComponent(speed_component);
    return *this;
  }

  GameObjectBuilder &AddMovement(const Vector2 direction = { 0.0F, 0.0F })
  {
    auto movement = std::make_shared<core::component::Movement>();
    movement->SetDirection(direction);
    obj_->AttachComponent(movement);
    return *this;
  }

  GameObjectBuilder &AddAnimation()
  {
    auto anim = std::make_shared<core::component::SpriteAnimation>();
    obj_->AttachComponent(anim);
    return *this;
  }

  GameObjectBuilder &
    AddCamera(const render::CameraMode mode, const float zoom, const float smoothness, const Vector2 deadzone)
  {
    auto camera = std::make_shared<core::component::CameraComponent>();
    camera->SetMode(mode);
    camera->SetZoom(zoom);
    camera->SetSmoothness(smoothness);
    camera->SetDeadzone(deadzone);

    obj_->AttachComponent(camera);
    return *this;
  }

  GameObjectBuilder &AddAudioListener()
  {
    auto listener = std::make_shared<core::component::AudioListener>();
    obj_->AttachComponent(listener);
    return *this;
  }

  GameObjectBuilder &AddHealth(float max_hp, const float current_hp)
  {
    auto health = std::make_shared<core::component::Health>(max_hp);
    health->SetCurrentHealth(current_hp);

    obj_->AttachComponent(health);
    return *this;
  }

  GameObjectBuilder &AddColoring(const Color color)
  {
    auto coloring = std::make_shared<core::component::Coloring>();
    coloring->SetColoring(color.r, color.g, color.b, color.a);

    obj_->AttachComponent(coloring);
    return *this;
  }

  GameObjectBuilder &AddRun(const bool is_running, const float speed_factor = 1.5F)
  {
    auto run = std::make_shared<core::component::Run>();
    run->SetRunning(is_running);
    run->SetSpeedFactor(speed_factor);

    obj_->AttachComponent(run);
    return *this;
  }

  GameObjectBuilder &AddInventory(const int capacity)
  {
    auto inventory = std::make_shared<core::component::Inventory>();
    inventory->SetCapacity(capacity);
    obj_->AttachComponent(inventory);
    return *this;
  }

  GameObjectBuilder &AddAttack(bool is_attacking)
  {
    auto attack = std::make_shared<core::component::Attack>(is_attacking);
    obj_->AttachComponent(attack);
    return *this;
  }

  GameObjectBuilder &AddProjectileType(const core::component::TypeProjectile projectile_type)
  {
    auto proj_type = std::make_shared<core::component::ProjectileType>();
    proj_type->SetProjectileType(projectile_type);
    obj_->AttachComponent(proj_type);
    return *this;
  }

  GameObjectBuilder &AddColliderBox(const GameContextType context_type,
    const Vector2 size,
    const Vector2 offset = { 0.0F, 0.0F },
    const collision::BodyType type = collision::BodyType::Dynamic)
  {
    auto collider = std::make_shared<core::component::ColliderBox>(size, offset);
    obj_->AttachComponent(collider);
    GCM::Instance().GetContext(context_type)->GetCollisionManager()->AddCollider(collider, type);
    return *this;
  }

  std::shared_ptr<T> Build() { return obj_; }
};

}// namespace chroma::shared::builder
