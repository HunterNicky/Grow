#include "chroma/shared/core/enemy/Enemy.h"

#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/enemy/EnemyAI.h"
#include "chroma/shared/core/components/world/EventColliderBox.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ProjectileEvent.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/render/SpriteLoader.h"
#include "chroma/shared/utils/UUID.h"

namespace chroma::shared::core::enemy {
Enemy::Enemy() { type_ = GameObjectType::ENEMY; }

Enemy::~Enemy() = default;

void Enemy::SetupAnimation() const
{
  auto anim_component = GetComponent<component::SpriteAnimation>();
  if (!anim_component) { return; }
  render::SpriteLoader::LoadSpriteAnimationFromFile(anim_component, "assets/sprites/enemy/enemy_hood.json");
  anim_component->Play("walk_down", true);
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
  const bool flip_x = (last_facing_ == EnemyFacingDir::Side) && last_left_;

  bridge->DrawAnimation(*anim, pos, scale, rotation, WHITE, flip_x, false, { 0.5F, 0.5F });
}

void Enemy::OnUpdate(const float delta_time)
{
  const auto enemy_ai = GetComponent<component::EnemyAI>();
  if (HasAuthority() && enemy_ai != nullptr) {
    enemy_ai->Update(delta_time);
  }

  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto speed = GetComponent<component::Speed>();
  if (!speed) { return; }

  const auto movement = GetComponent<component::Movement>();
  if (!movement) { return; }

  const bool should_simulate = HasAuthority() || IsAutonomousProxy();

  Vector2 dir = movement->GetDirection();
  const float magnitude = Vector2Length(dir);

  if (magnitude > 0.0F) {
    dir = Vector2Normalize(dir);
    movement->SetDirection(dir);

    if (should_simulate) {
      Vector2 pos = transform->GetPosition();
      pos.x += dir.x * speed->GetSpeed().x * delta_time;
      pos.y += dir.y * speed->GetSpeed().y * delta_time;
      transform->SetPosition(pos);
    }
  }
  UpdateRangedAttack(delta_time, dir);

  UpdateAnimations(dir, magnitude);
}

void Enemy::UpdateRangedAttack(const float delta_time, const Vector2 &dir)
{
  (void)dir;

  if (!HasAuthority()) { return; }

  last_attack_time_ += delta_time;
  if (last_attack_time_ < attack_cooldown_) { return; }

  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const Vector2 my_pos = transform->GetPosition();
  // Busca o player mais próximo de forma segura
  const auto context = GCM::Instance().GetContext(GameContextType::Server);
  if (!context) { return; }
  const auto manager = context->GetGameObjectManager();
  if (!manager) { return; }

  const auto &players = manager->GetByTag(GameObjectType::PLAYER);
  if (players.empty()) { return; }

  float best_dist_sq = std::numeric_limits<float>::max();
  std::shared_ptr<component::Transform> best_tr;

  for (const auto &p : players) {
    if (!p) { continue; }
    const auto p_tr = p->GetComponent<component::Transform>();
    if (!p_tr) { continue; }
    const Vector2 pos = p_tr->GetPosition();
    const float dx = pos.x - my_pos.x;
    const float dy = pos.y - my_pos.y;
    const float dist_sq = (dx * dx) + (dy * dy);
    if (dist_sq < best_dist_sq) {
      best_dist_sq = dist_sq;
      best_tr = p_tr;
    }
  }

  if (!best_tr) { return; }

  const Vector2 player_pos = best_tr->GetPosition();
  Vector2 shot_dir = Vector2Subtract(player_pos, my_pos);
  const float dist = Vector2Length(shot_dir);
  if (dist <= 0.0F) { return; }

  constexpr float attack_range = 70.0F;
  if (dist > attack_range) { return; }

  shot_dir = Vector2Normalize(shot_dir);

  auto projectile_event = std::make_shared<event::ProjectileEvent>(component::TypeProjectile::ARROW, shot_dir, 100.0F);
  projectile_event->SetProjectileId(utils::UUID::Generate());
  projectile_event->SetPosition(my_pos);
  event::EventBus::Dispatch(*projectile_event);

  last_attack_time_ = 0.0F;
}

void Enemy::UpdateAnimations(const Vector2 &dir, const float magnitude)
{
  const auto transform = GetComponent<component::Transform>();
  if (!transform) { return; }

  const auto anim = GetComponent<component::SpriteAnimation>();
  if (!anim) { return; }

  if (magnitude > 0.0F) {
    if (std::fabs(dir.x) > std::fabs(dir.y)) {
      last_facing_ = EnemyFacingDir::Side;
      last_left_ = (dir.x < 0.0F);
      anim->Play("walk_side", true);
    } else if (dir.y < 0.0F) {
      last_facing_ = EnemyFacingDir::Up;
      anim->Play("walk_up", true);
    } else {
      last_facing_ = EnemyFacingDir::Down;
      anim->Play("walk_down", true);
    }
  } else {
    Vector2 to_player{ 0.0F, 0.0F };
    const auto context = GCM::Instance().GetContext(GameContextType::Server);
    if (context) {
      const auto manager = context->GetGameObjectManager();
      if (manager) {
        const auto &players = manager->GetByTag(GameObjectType::PLAYER);
        if (!players.empty()) {
          const Vector2 my_pos = transform->GetPosition();
          float best_dist_sq = std::numeric_limits<float>::max();
          std::shared_ptr<component::Transform> best_tr;

          for (const auto &p : players) {
            if (!p) { continue; }
            const auto p_tr = p->GetComponent<component::Transform>();
            if (!p_tr) { continue; }
            const Vector2 pos = p_tr->GetPosition();
            const float dx = pos.x - my_pos.x;
            const float dy = pos.y - my_pos.y;
            const float dist_sq = (dx * dx) + (dy * dy);
            if (dist_sq < best_dist_sq) {
              best_dist_sq = dist_sq;
              best_tr = p_tr;
            }
          }

          if (best_tr) {
            to_player = Vector2Subtract(best_tr->GetPosition(), my_pos);
          }
        }
      }
    }

    if (to_player.x == 0.0F && to_player.y == 0.0F) {
      last_facing_ = EnemyFacingDir::Down;
      anim->Play("walk_down", true);
    } else {
      if (std::fabs(to_player.x) > std::fabs(to_player.y)) {
        last_facing_ = EnemyFacingDir::Side;
        last_left_ = (to_player.x < 0.0F);
        anim->Play("attack_side", true);
      } else if (to_player.y < 0.0F) {
        last_facing_ = EnemyFacingDir::Up;
        anim->Play("attack_up", true);
      } else {
        last_facing_ = EnemyFacingDir::Down;
        anim->Play("attack_down", true);
      }
    }
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