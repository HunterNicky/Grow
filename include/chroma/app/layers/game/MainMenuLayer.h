#pragma once

#include <memory>
#include <string>

#include "chroma/app/layers/Layer.h"
#include "chroma/shared/events/state/PopStateEvent.h"
#include "chroma/shared/events/state/PushStateEvent.h"
#include "chroma/shared/events/Subscription.h"

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

  void OnPopState(const shared::event::state::PopStateEvent& event);
  void OnPushState(shared::event::state::PushStateEvent& event);

  void DoPopState();
  void DoPushState(const std::string& state_id);

private:
  shared::event::Subscription pop_state_sub_;
  shared::event::Subscription push_state_sub_;
};
}// namespace chroma::app::layer::game
