#pragma once

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::network {
class NetworkLayer : public Layer
{
public:
  NetworkLayer();

  void OnUpdate(float delta_time) override;
  void OnEvent(event::Event& event) override;
};
}