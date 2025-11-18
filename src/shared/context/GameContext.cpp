#include "chroma/shared/context/GameContext.h"

namespace chroma::shared::context {

GameContext::GameContext()
    : player_health_(std::make_shared<float>(100.0F)),
      player_max_health_(std::make_shared<float>(100.0F)),
      delta_time_(std::make_shared<float>(0.0F)) {}

GameContext& GameContext::GetInstance() {
    static GameContext instance; 
    return instance;
}

void GameContext::SetPlayerHealth(float health) {
    *player_health_ = health;
}

std::shared_ptr<float> GameContext::GetPlayerHealth() {
    return player_health_;
}

void GameContext::SetDeltaTime(float delta_time) {
    *delta_time_ = delta_time;
}

std::shared_ptr<float> GameContext::GetDeltaTime() {
    return delta_time_;
}

std::shared_ptr<float> GameContext::GetPlayerMaxHealth() {
    return player_max_health_;
}

void GameContext::SetPlayerMaxHealth(float max_health) {
    *player_max_health_ = max_health;
}

} // namespace chroma::shared::context