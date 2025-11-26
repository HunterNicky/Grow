#include "chroma/shared/core/enemy/Enemy.h"

#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/enemy/EnemyAI.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/world/EventColliderBox.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/render/SpriteLoader.h"

namespace chroma::shared::core::enemy {
Enemy::Enemy() { type_ = GameObjectType::ENEMY; }

Enemy::~Enemy() = default;

void Enemy::SetupAnimation() const
{
  auto anim_component = GetComponent<component::SpriteAnimation>();
  if (!anim_component) { return; }
  render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/enemy/enemy_hood.json");
  anim_component->Play("die", true);
}

void Enemy::OnRender()
{
  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  const auto bridge = render::GetRenderBridge();
  if (!bridge) { return; }

  const Vector2 pos = transform->GetPosition();
  const Vector2 scale = transform->GetScale();
  const float rotation = transform->GetRotation();

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, false, false, { 0.5F, 0.5F });
}

void Enemy::OnUpdate(const float delta_time)
{
  const auto enemy_ai = GetComponent<component::EnemyAI>();
  if (HasAuthority() && enemy_ai != nullptr) {
    const auto target =
      GCM::Instance().GetContext(GameContextType::Server)->GetGameObjectManager()->GetByTag(GameObjectType::PLAYER);
    if (target.empty()) {
      enemy_ai->SetTarget(nullptr);
    } else {
      enemy_ai->SetTarget(target.front());
    }
    enemy_ai->Update(delta_time);
  }
}

std::shared_ptr<GameObject> Enemy::Clone() { return std::make_shared<Enemy>(*this); }

void Enemy::OnFixedUpdate([[maybe_unused]] float fixed_delta_time) {}
void Enemy::OnCollision(const collision::CollisionEvent &collision)
{
  const auto other = collision.other.lock();
  if (!other) { return; }

  const auto my_health = GetComponent<component::Health>();
  const auto other_health = other->GetComponent<component::Health>();

  const auto my_event_collider = GetComponent<component::EventColliderBox>();

    if (collision.type == collision::CollisionEvent::Type::Trigger && my_event_collider && other_health
      && other->GetTag() == GameObjectType::PLAYER) {
    other_health->TakeDamage(2.0F);
  }

  (void)my_health;
}

void Enemy::HandleEvent([[maybe_unused]] event::Event &event) {}
}// namespace chroma::shared::core::enemy