#pragma once 

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::states {
class NetWorkLayer : public Layer
{
public:        
    NetWorkLayer();
    ~NetWorkLayer() override = default;
    void OnUpdate(const float delta_time) override;
    void OnRender() override;
};
}// namespace chroma::app::layer::states