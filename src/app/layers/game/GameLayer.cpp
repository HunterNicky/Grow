#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/app/layers/Layer.h"

#include <iostream>

namespace chroma::app::layer::game {

GameLayer::GameLayer() : Layer("GameLayer") {}

GameLayer::~GameLayer() { std::cout << "GameLayer destroyed\n"; }

void GameLayer::OnUpdate(const float delta_time)
{
  if (!IsActive()) { return; }

  auto current_state = GetCurrentState();

  if (current_state) { current_state->OnUpdate(delta_time); }
}

void GameLayer::OnRender()
{
  if (!IsActive()) { return; }

  auto current_state = GetCurrentState();

  if (current_state) { current_state->OnRender(); }
}

}// namespace chroma::app::layer::game
