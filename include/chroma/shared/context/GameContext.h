#pragma once

#include <memory>

#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::collision {
class CollisionManager;
}
namespace chroma::shared::core {
class GameObjectManager;
}
namespace chroma::shared::context {

enum class GameContextType : uint8_t { Client = 0, Server = 1 };

class GameContext
{
public:
  GameContext();
  ~GameContext() = default;

  GameContext(const GameContext &) = delete;
  GameContext &operator=(const GameContext &) = delete;
  GameContext(GameContext &&) = delete;
  GameContext &operator=(GameContext &&) = delete;

  void SetLocalPlayer(std::shared_ptr<core::GameObject> p);
  [[nodiscard]] std::shared_ptr<core::GameObject> GetLocalPlayer() const;

  void RemoveLocalPlayer(std::shared_ptr<core::GameObject> p);

  void SetDeltaTime(float delta_time) const;
  [[nodiscard]] std::shared_ptr<float> GetDeltaTime() const;

  void SetType(GameContextType type);
  [[nodiscard]] GameContextType GetType() const;

  [[nodiscard]] std::shared_ptr<core::GameObjectManager> GetGameObjectManager() const;
  [[nodiscard]] std::shared_ptr<collision::CollisionManager> GetCollisionManager() const;

  void SetGameObjectManager(const std::shared_ptr<core::GameObjectManager> &manager);
  void SetCollisionManager(const std::shared_ptr<collision::CollisionManager> &manager);

private:
  GameContextType type_{ GameContextType::Client };

  std::shared_ptr<core::GameObjectManager> game_objects_;
  std::shared_ptr<collision::CollisionManager> collisions_;

  std::shared_ptr<core::GameObject> local_player_;
  std::shared_ptr<float> delta_time_;
};
}// namespace chroma::shared::context

using GameContextType = chroma::shared::context::GameContextType;
