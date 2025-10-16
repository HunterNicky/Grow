#pragma once 

#include "chroma/app/layers/states/State.h"
#include <vector>
#include "chroma/shared/core/GameObject.h"

namespace chroma::app::layer::states {
class GameState : public State
{
public:
    GameState();
    ~GameState() override = default;

    void OnRender() override;
    void OnUpdate(float delta_time) override;

private:
    std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_;
};
} // namespace chroma::app::layer::states nicky gay
