#include "chroma/shared/core/projectile/Projectile.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/ProjectileType.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/render/SpriteLoader.h"

#include <memory>
#include <raylib.h>
#include <string>

namespace chroma::shared::core::projectile {
Projectile::Projectile() { Type_ = GameObjectType::PROJECTILE; }

Projectile::~Projectile() = default;

void Projectile::OnRender()
{
  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }

  const Vector2 pos = transform->GetPosition();
  const Vector2 scale = transform->GetScale();
  const float rotation = transform->GetRotation();
  const bool flip_x = (movement->GetDirection().x < 0.0F);

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, flip_x, false, { 0.5F, 0.5F });
}

void Projectile::OnUpdate(float delta_time)
{
  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }

  const auto speed = GetComponent<component::Speed>();
  if (!speed) { return; }

  Vector2 pos = transform->GetPosition();
  const Vector2 dir = movement->GetDirection();
  const Vector2 speed_value = speed->GetSpeed();

  pos.x += dir.x * speed_value.x * delta_time;
  pos.y += dir.y * speed_value.y * delta_time;

  transform->SetPosition(pos);
  UpdateAnimation();
}

void Projectile::InitAnimation()
{
  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto projectile_type = GetComponent<component::ProjectileType>();
  if (!projectile_type) { return; }

  std::string sprite_path;

  if (projectile_type->GetProjectileType() == component::TypeProjectile::ARROW) {
    sprite_path = "assets/sprites/player/projectile/arrow-projectile.json";
  } else if (projectile_type->GetProjectileType() == component::TypeProjectile::JAVELIN) {
    sprite_path = "assets/sprites/player/projectile/javelin-projectile.json";
  }

  render::SpriteLoader::LoadSpriteAnimationFromFile(anim, sprite_path);
}

void Projectile::UpdateAnimation()
{
  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }

  if (movement->GetDirection().x == 1.0F || movement->GetDirection().x == -1.0F) {
    anim->Play("projectile_attack_horizontal", true);
  } else if (movement->GetDirection().y == 1.0F || movement->GetDirection().y == -1.0F) {
    anim->Play("projectile_attack_vertical", true);
  }
}

std::shared_ptr<GameObject> Projectile::Clone() { return std::make_shared<Projectile>(*this); }

void Projectile::OnFixedUpdate(float delta_time) { (void)delta_time; }

void Projectile::OnCollision(const collision::CollisionEvent &event) { (void)event; }

void Projectile::HandleEvent(event::Event &event) { (void)event; }

}// namespace chroma::shared::core::projectile