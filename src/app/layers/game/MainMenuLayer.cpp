#include <memory>
#include <string>
#include <utility>

#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/game/MainMenuLayer.h"
#include "chroma/app/states/StateFactory.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/MainMenuState.h"
#include "chroma/app/states/menu/OptionsMenuState.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/ui/PanelEvent.h"
#include "chroma/shared/events/EventBus.h"

namespace chroma::app::layer::game {
MainMenuLayer::MainMenuLayer(std::string name) : Layer(std::move(name))
{
  auto main_menu_state = std::make_shared<chroma::app::states::menu::MainMenuState>();
  state_machine_->PushState(main_menu_state);
}

MainMenuLayer::~MainMenuLayer()
{
  shared::event::ui::PanelEvent bg_main(
    shared::event::ui::Action::Close, client::ui::panel::PanelID::MainBackgroundPanel);
  shared::event::EventBus::Dispatch(bg_main);
};

void MainMenuLayer::RegisterStates()
{
  state_factory_.Register(
    states::StateID::MainMenuState, []() { return std::make_shared<states::menu::MainMenuState>(); });

  state_factory_.Register(
    states::StateID::OptionsMenuState, []() { return std::make_shared<states::menu::OptionsMenuState>(); });
}
}// namespace chroma::app::layer::game