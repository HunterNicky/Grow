#include <iostream>
#include <memory>
#include <raylib.h>

#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/layers/LayerIdentifiers.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/menu/PauseState.h"
#include "chroma/client/render/Window.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/components/Health.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/layer/LayerEvent.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Nivel.h"

#include "chroma/shared/events/PlayerDataEvent.h"

namespace chroma::app::states::menu {
PauseState::PauseState() : State("PauseState")
{
  shared::event::ui::PanelEvent panel_event(shared::event::ui::Action::Open, client::ui::panel::PanelID::PausePanel);
  shared::event::EventBus::Dispatch(panel_event);

  button_click_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ui::ButtonClickEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

PauseState::~PauseState()
{
  shared::event::ui::PanelEvent panel_event(shared::event::ui::Action::Close, client::ui::panel::PanelID::PausePanel);
  shared::event::EventBus::Dispatch(panel_event);
}

void PauseState::OnUpdate(const float delta_time) { (void)delta_time; }

void PauseState::OnRender() {}

void PauseState::OnEvent(shared::event::Event &event)
{
  auto *btn_event = dynamic_cast<shared::event::ui::ButtonClickEvent *>(&event);
  if (!btn_event) { return; }

  const auto &id = btn_event->GetId();
  auto dispatch = [](auto &&e) { shared::event::EventBus::Dispatch(e); };

  using StateAction = shared::event::state::Action;
  using LayerAction = shared::event::layer::Action;
  using LayerID = layer::LayerID;
  using UIAction = shared::event::ui::Action;
  using PanelID = client::ui::panel::PanelID;

  if (id == "Resume") {
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::PauseState));
    dispatch(shared::event::ui::PanelEvent(UIAction::Close, PanelID::PauseBackgroundPanel));
  } else if (id == "Options") {
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::PauseState));
    dispatch(shared::event::state::StateEvent(StateAction::Push, StateID::OptionsMenuState));
  } else if (id == "MainMenu") {
    dispatch(shared::event::layer::LayerEvent(LayerAction::Pop, LayerID::GameLayer));
    dispatch(shared::event::state::StateEvent(StateAction::Pop, StateID::NetworkState));
    dispatch(shared::event::layer::LayerEvent(LayerAction::Push, LayerID::MenuLayer));
    dispatch(shared::event::ui::PanelEvent(UIAction::Close, PanelID::PauseBackgroundPanel));
  } else if (id == "Save") {

    database::PlayerData player_data;
    auto player = std::dynamic_pointer_cast<shared::core::player::Player>(GCM::Instance().GetContext(GameContextType::Client)->GetLocalPlayer());

    player_data.character_id = static_cast<int>(player->GetTag());
    player_data.coins = 0;

    player_data.current_xp = 0;
    if (auto lvl = player->GetComponent<shared::core::component::Nivel>()) {
        player_data.current_xp = static_cast<int>(lvl->GetExperience());
    }

    player_data.direction = static_cast<int>(player->GetLastFacingDir());

    player_data.hp = 0;
    if (auto health = player->GetComponent<shared::core::component::Health>()) {
        player_data.hp = static_cast<int>(*health->GetCurrentHealth());
    }

    player_data.level = 0;
    if (auto lvl = player->GetComponent<shared::core::component::Nivel>()) {
        player_data.level = lvl->GetNivel();
    }

    player_data.pos_x = 0;
    player_data.pos_y = 0;
    if (auto tr = player->GetComponent<shared::core::component::Transform>()) {
        player_data.pos_x = static_cast<int>(tr->GetPosition().x);
        player_data.pos_y = static_cast<int>(tr->GetPosition().y);
    }

    player_data.weapon_id = 0;
    if (auto inv = player->GetComponent<shared::core::component::Inventory>()) {
        if (auto weapon = inv->GetCurrentWeapon()) {
            player_data.weapon_id = static_cast<int>(weapon->GetWeaponType());
        }
    }

    dispatch(shared::event::PlayerDataEvent(shared::event::PlayerDataAction::Save, player_data));
  }
}
}// namespace chroma::app::states::menu