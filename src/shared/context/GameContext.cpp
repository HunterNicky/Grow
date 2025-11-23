#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/GameObject.h"

#include <memory>
#include <utility>

namespace chroma::shared::context {

GameContext::GameContext() : delta_time_(std::make_shared<float>(0.0F)) {}

GameContext &GameContext::GetInstance()
{
  static GameContext instance;
  return instance;
}

void GameContext::SetDeltaTime(float delta_time) { *delta_time_ = delta_time; }

std::shared_ptr<float> GameContext::GetDeltaTime() const { return delta_time_; }

void GameContext::SetLocalPlayer(std::shared_ptr<core::GameObject> p) { local_player_ = std::move(p); }

std::shared_ptr<core::GameObject> GameContext::GetLocalPlayer() const { return local_player_; }

}// namespace chroma::shared::context