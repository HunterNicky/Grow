#include "chroma/shared/core/components/enemy/EnemyAI.h"

#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/core/components/world/WorldNavigation.h"

namespace chroma::shared::core::component {

EnemyAI::EnemyAI() = default;

void EnemyAI::Initialize()
{
  blackboard_.owner = game_object_;

  if (const auto owner = blackboard_.owner.lock()) {
    const auto context = context::GameContextManager::Instance().GetContext(GameContextType::Server);
    if (context) {
      const auto manager = context->GetGameObjectManager();
      if (manager) {
        const auto &worlds = manager->GetByTag(GameObjectType::WORLD);
        if (!worlds.empty() && worlds.front()) {
          const auto world_nav = worlds.front()->GetComponent<WorldNavigation>();
          if (world_nav) {
            blackboard_.nav_grid = &world_nav->GetGrid();
            blackboard_.grid_width = world_nav->GetWidth();
            blackboard_.grid_height = world_nav->GetHeight();
            blackboard_.tile_size = world_nav->GetTileSize();
          }
        }
      }
    }
  }

  // Construção da Árvore
  // ROOT (Selector)
  //  |-- Sequence (Perseguir)
  //  |    |-- Check (Tem alvo?)
  //  |    '-- Task (Persegue)
  //  '-- Task (Idle - se não tiver alvo)
  using namespace aitoolkit::bt;

  root_node_ = std::make_unique<sel<ai::EnemyBlackboard>>(node_list<ai::EnemyBlackboard>(
    seq<ai::EnemyBlackboard>(node_list<ai::EnemyBlackboard>(
      check<ai::EnemyBlackboard>(ai::CheckHasTarget{}), task<ai::EnemyBlackboard>(ai::TaskChaseTarget{}))),
    task<ai::EnemyBlackboard>(ai::TaskIdle{})));
}

void EnemyAI::SetTarget(const std::shared_ptr<GameObject> &target) { blackboard_.target = target; }

void EnemyAI::Update([[maybe_unused]] float delta_time)
{
  if (const auto owner = blackboard_.owner.lock()) {
    const auto owner_tr = owner->GetComponent<Transform>();
    if (owner_tr) {
      const auto context = context::GameContextManager::Instance().GetContext(GameContextType::Server);
      if (context) {
        const auto manager = context->GetGameObjectManager();
        if (manager) {
          const auto &players = manager->GetByTag(GameObjectType::PLAYER);
          float best_dist_sq = std::numeric_limits<float>::max();
          std::shared_ptr<GameObject> best_player;

          const Vector2 my_pos = owner_tr->GetPosition();
          for (const auto &p : players) {
            if (!p) { continue; }
            const auto p_tr = p->GetComponent<Transform>();
            if (!p_tr) { continue; }
            const Vector2 pos = p_tr->GetPosition();
            const float dx = pos.x - my_pos.x;
            const float dy = pos.y - my_pos.y;
            const float dist_sq = (dx * dx) + (dy * dy);
            if (dist_sq < best_dist_sq) {
              best_dist_sq = dist_sq;
              best_player = p;
            }
          }

          blackboard_.target = best_player;
        }
      }
    }
  }

  if (root_node_) { root_node_->evaluate(blackboard_); }
}
}// namespace chroma::shared::core::component