#pragma once
#include "chroma/shared/ai/EnemyBlackboard.h"
#include "chroma/shared/core/components/Component.h"

#include <aitoolkit/behtree.hpp>
#include <memory>
#include <raylib.h>

namespace chroma::shared::core::component {

class EnemyAI final : public Component
{
public:
  explicit EnemyAI();

  void Initialize();
  void SetTarget(const std::shared_ptr<GameObject> &target);
  void Update(float delta_time) override;

  [[nodiscard]] const ai::EnemyBlackboard &GetBlackboard() const { return blackboard_; }

private:
  static std::shared_ptr<GameObject> FindClosestPlayer(const Vector2 &my_pos);

  ai::EnemyBlackboard blackboard_;
  std::unique_ptr<aitoolkit::bt::node<ai::EnemyBlackboard>> root_node_;
};
}// namespace chroma::shared::core::component