#pragma once

#include <memory>

#include "chroma/shared/core/GameObject.h"

namespace chroma::shared::context {

class GameContext {
public:
    static GameContext& GetInstance();

    ~GameContext() = default;
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;
    GameContext(GameContext&&) = delete;
    GameContext& operator=(GameContext&&) = delete;
    
    void SetLocalPlayer(std::shared_ptr<core::GameObject> p);
    [[nodiscard]]std::shared_ptr<core::GameObject> GetLocalPlayer() const;

    void SetDeltaTime(float delta_time);
    [[nodiscard]] std::shared_ptr<float> GetDeltaTime() const;

private:
    GameContext();

    std::shared_ptr<core::GameObject> local_player_;
     std::shared_ptr<float> delta_time_;
};
} // namespace chroma::shared::context
