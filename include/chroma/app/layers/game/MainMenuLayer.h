#pragma once

#include <memory>
#include <string>

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::game {
class MainMenuLayer : public Layer
{
public:
  explicit MainMenuLayer(std::string name);
  ~MainMenuLayer() override;

  MainMenuLayer(const MainMenuLayer &) = delete;
  MainMenuLayer &operator=(const MainMenuLayer &) = delete;
  MainMenuLayer(MainMenuLayer &&) = delete;
  MainMenuLayer &operator=(MainMenuLayer) = delete;

  void OnStateEvent(shared::event::state::StateEvent &state_event) override;

  void RegisterStates() override;
};
}// namespace chroma::app::layer::game
