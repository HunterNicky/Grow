#include "chroma/shared/core/components/enemy/EnemyAI.h"

namespace chroma::shared::core::component {

EnemyAIComponent::EnemyAIComponent(const std::shared_ptr<GameObject> &owner)
{
  blackboard_.owner = owner;

  using namespace aitoolkit::bt;

  // Construção da Árvore
  // ROOT (Selector)
  //  |-- Sequence (Perseguir)
  //  |    |-- Check (Tem alvo?)
  //  |    '-- Task (Persegue)
  //  '-- Task (Idle - se não tiver alvo)

  root_node_ = std::make_unique<sel<ai::EnemyBlackboard>>(node_list<ai::EnemyBlackboard>(
    seq<ai::EnemyBlackboard>(node_list<ai::EnemyBlackboard>(
      check<ai::EnemyBlackboard>(ai::CheckHasTarget{}), task<ai::EnemyBlackboard>(ai::TaskChaseTarget{}))),
    task<ai::EnemyBlackboard>(ai::TaskIdle{})));
}

void EnemyAIComponent::SetTarget(const std::shared_ptr<GameObject> &target) { blackboard_.target = target; }

void EnemyAIComponent::Update([[maybe_unused]] float delta_time)
{
  if (root_node_) { root_node_->evaluate(blackboard_); }
}
}// namespace chroma::shared::core::component