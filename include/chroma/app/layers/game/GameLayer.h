#pragma once

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::game {
class GameLayer : public Layer
{
public:
  GameLayer();
  ~GameLayer() override;

  GameLayer(const GameLayer &) = delete;
  GameLayer &operator=(const GameLayer &) = delete;
  GameLayer(GameLayer &&) = delete;
  GameLayer &operator=(GameLayer &&) = delete;

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
};
}// namespace chroma::app::layer::game
