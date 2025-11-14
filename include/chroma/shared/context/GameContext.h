#pragma once

#include <memory>

namespace chroma::shared::context {

class GameContext {
public:
    static GameContext& GetInstance();

    ~GameContext() = default;
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;
    GameContext(GameContext&&) = delete;
    GameContext& operator=(GameContext&&) = delete;

    void SetPlayerHealth(float health);
    void SetPlayerMaxHealth(float max_health);
    void SetDeltaTime(float delta_time);

    std::shared_ptr<float> GetPlayerHealth();
    std::shared_ptr<float> GetPlayerMaxHealth();
    std::shared_ptr<float> GetDeltaTime();

private:
    GameContext() = default;

    std::shared_ptr<float> player_health_ = std::make_shared<float>(100.0F);
    std::shared_ptr<float> player_max_health_ = std::make_shared<float>(100.0F);
    std::shared_ptr<float> delta_time_ = std::make_shared<float>(0.0F);
};

} // namespace chroma::shared::context
