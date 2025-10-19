#pragma once 

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::game {
class GameLayer : public Layer
{
public:        
    GameLayer();
    void OnUpdate(const float delta_time) override;
    void OnRender() override;
    void OnEvent(event::Event& event) override;
};
}// namespace chroma::app::layer::game
