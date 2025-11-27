#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/collision/CollisionManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"

#include <memory>
#include <raylib.h>
#include <utility>

namespace chroma::shared::context {

GameContext::GameContext()
  : game_objects_(std::make_shared<core::GameObjectManager>()),
    collisions_(std::make_shared<collision::CollisionManager>(Rectangle{ 0.0F, 0.0F, 1920.0F, 1080.0F }, type_)),
    delta_time_(std::make_shared<float>(0.0F))
{
  game_objects_->SetCollisionManager(collisions_);
}

void GameContext::SetDeltaTime(const float delta_time) const { *delta_time_ = delta_time; }

std::shared_ptr<float> GameContext::GetDeltaTime() const { return delta_time_; }

void GameContext::SetType(const GameContextType type)
{
  type_ = type;
  if (collisions_) { collisions_->SetContextType(type_); }
}

GameContextType GameContext::GetType() const { return type_; }

std::shared_ptr<core::GameObjectManager> GameContext::GetGameObjectManager() const { return game_objects_; }

std::shared_ptr<collision::CollisionManager> GameContext::GetCollisionManager() const { return collisions_; }

void GameContext::SetGameObjectManager(const std::shared_ptr<core::GameObjectManager> &manager)
{
  game_objects_ = manager;
  game_objects_->SetCollisionManager(collisions_);
  collisions_->SetContextType(type_);
}

void GameContext::SetCollisionManager(const std::shared_ptr<collision::CollisionManager> &manager)
{
  collisions_ = manager;
  game_objects_->SetCollisionManager(collisions_);
  collisions_->SetContextType(type_);
}

void GameContext::SetLocalPlayer(std::shared_ptr<core::GameObject> p) { local_player_ = std::move(p); }

std::shared_ptr<core::GameObject> GameContext::GetLocalPlayer() const { return local_player_; }
}// namespace chroma::shared::context