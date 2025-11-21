#pragma once

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::network {
class NetworkLayer : public Layer
{
public:
  NetworkLayer();
  ~NetworkLayer() override;

  NetworkLayer(const NetworkLayer &) = delete;
  NetworkLayer &operator=(const NetworkLayer &) = delete;
  NetworkLayer(NetworkLayer &&) = delete;
  NetworkLayer &operator=(NetworkLayer &&) = delete;
  void OnUpdate(float delta_time) override;
  void RegisterStates() override;
};
}// namespace chroma::app::layer::network