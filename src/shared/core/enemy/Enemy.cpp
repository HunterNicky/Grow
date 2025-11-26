#include "chroma/shared/core/enemy/Enemy.h"
#include "chroma/shared/core/components/enemy/EnemyAI.h"

namespace chroma::shared::core::enemy {
Enemy::Enemy() { type_ = GameObjectType::ENEMY; }

Enemy::~Enemy() = default;

void Enemy::OnRender() {}

void Enemy::OnUpdate([[maybe_unused]] float delta_time) {}

std::shared_ptr<GameObject> Enemy::Clone() { return std::make_shared<Enemy>(*this); }

void Enemy::OnFixedUpdate([[maybe_unused]] float fixed_delta_time) {}

void Enemy::OnCollision([[maybe_unused]] const collision::CollisionEvent &collision) {}

void Enemy::HandleEvent([[maybe_unused]] event::Event &event) {}
}// namespace chroma::shared::core::enemy