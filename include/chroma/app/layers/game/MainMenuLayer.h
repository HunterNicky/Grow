#pragma once

#include <memory>
#include <string>

#include "chroma/app/layers/Layer.h"
#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/Event.h"
#include "chroma/app/states/StateIdentifiers.h"

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
  
  void OnEvent(shared::event::Event &event) override;
  void DoPushState(const states::StateID state_id);

private:
  shared::event::Subscription state_sub_;
};
}// namespace chroma::app::layer::game
