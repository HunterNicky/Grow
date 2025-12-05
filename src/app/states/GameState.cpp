#include "chroma/app/states/GameState.h"
#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Bow.h"
#include "chroma/shared/core/components/CharacterType.h"
#include "chroma/shared/core/components/Fist.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Javelin.h"
#include "chroma/shared/core/components/Spear.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/ProjectileEvent.h"
#include "chroma/shared/events/ShaderEvent.h"
#include "chroma/shared/events/SoundEvent.h"
#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/shared/events/ui/PanelEvent.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/utils/Random.h"
#include "chroma/shared/utils/UUID.h"

#include <cstdint>
#include <memory>
#include <raylib.h>
#include <utility>
#include <uuid_v4.h>

namespace chroma::app::states {

GameState::GameState(shared::core::component::CharacterType character_type)
  : State("GameState"), network_mediator_(nullptr)
{

  if (character_type == shared::core::component::CharacterType::NONE) {
    character_type = static_cast<shared::core::component::CharacterType>(shared::utils::Random::Int(1, 2));
  }

  auto player = chroma::shared::builder::GameObjectBuilder<shared::core::player::Player>()
                  .AddTransform({ 0, 0 })
                  .Id(shared::utils::UUID::Generate())
                  .AddSpeed(50.0F)
                  .AddMovement()
                  .AddAnimation()
                  .AddCamera(shared::render::CameraMode::FollowSmooth, 3.0F, 2.0F, { 64, 128 })
                  .AddColliderBox(GameContextType::Server, { 16.F, 32.F }, { -8.F, -16.F })
                  .AddAudioListener()
                  .AddHealth(100.0F, 1.0F)
                  .AddRun(false, 1.5F)
                  .AddInventory(10)
                  .AddAttack(false)
                  .AddCharacterType(character_type)
                  .AddNivel(1, 0.0F, 100.0F)
                  .NetRole(shared::core::NetRole::AUTONOMOUS)
                  .Build();

  switch (player->GetComponent<shared::core::component::CharacterTypeComponent>()->GetCharacterType()) {
  case shared::core::component::CharacterType::RANDI: {
    auto spear = std::make_shared<shared::core::component::Spear>();
    auto javelin = std::make_shared<shared::core::component::Javelin>();
    player->GetComponent<shared::core::component::Inventory>()->AddInventory(spear);
    player->GetComponent<shared::core::component::Inventory>()->AddInventory(javelin);
    break;
  }
  case shared::core::component::CharacterType::PRIMM: {
    auto bow = std::make_shared<shared::core::component::Bow>();
    player->GetComponent<shared::core::component::Inventory>()->AddInventory(bow);
    player->SetCurrentWeapon(bow);
    break;
  }
  default:
    break;
  }

  auto fist = std::make_shared<shared::core::component::Fist>();

  player->GetComponent<shared::core::component::Inventory>()->AddInventory(fist);
  player->SetupAnimation(player->GetComponent<shared::core::component::SpriteAnimation>());

  GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Register(player);
  SetPlayerId(player->GetId());

  shared::event::ui::PanelEvent hud_main(shared::event::ui::Action::Open, client::ui::panel::PanelID::GameHUDPanel);
  shared::event::EventBus::Dispatch(hud_main);
}

GameState::GameState(std::shared_ptr<GameNetworkMediator> network_mediator)
  : State("GameState"), network_mediator_(std::move(network_mediator))
{

  shared::event::ui::PanelEvent hud_main(shared::event::ui::Action::Open, client::ui::panel::PanelID::GameHUDPanel);
  shared::event::EventBus::Dispatch(hud_main);
}

GameState::~GameState()
{
  GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Clear();

  shared::event::ui::PanelEvent hud_main(shared::event::ui::Action::Close, client::ui::panel::PanelID::GameHUDPanel);
  shared::event::EventBus::Dispatch(hud_main);

  shared::event::ShaderEvent shader_event(shared::event::ShaderEventType::REMOVE);
  shader_event.SetPassType(client::render::shader::PassType::HEALTH);
  shader_event.SetFront(true);
  shared::event::EventBus::GetDispatcher()->Dispatch(shader_event);
}

void GameState::OnRender()
{
  GCM::Instance()
    .GetContext(GameContextType::Client)
    ->GetGameObjectManager()
    ->ForEach([](const std::shared_ptr<shared::core::GameObject> &obj) {
      if (obj && obj->IsActive()) { obj->OnRender(); }
    });
}

void GameState::OnUpdate(float delta_time)
{
  if (!IsActive()) { return; }
  if (network_mediator_) { network_mediator_->UpdateInterpolation(static_cast<uint64_t>(delta_time * 1000)); }

  GCM::Instance()
    .GetContext(GameContextType::Client)
    ->GetGameObjectManager()
    ->ForEach([delta_time](const std::shared_ptr<shared::core::GameObject> &obj) {
      if (obj && obj->IsActive()) { obj->OnUpdate(delta_time); }
    });

  if (const auto collision_manager =
        GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->GetCollisionManager();
    collision_manager) {
    collision_manager->Update();
  }
}

void GameState::OnEvent(shared::event::Event &event)
{
  if (player_id_ == UUIDv4::UUID{}) { return; }
  if (IsPaused()) { return; }

  if (const auto *key_event = dynamic_cast<shared::event::KeyEvent *>(&event)) {
    if (key_event->IsPressed() && key_event->GetKey() == KEY_P) {
      SetPaused(true);
      shared::event::state::StateEvent state_event(shared::event::state::Action::Push, StateID::PauseState);
      shared::event::EventBus::Dispatch(state_event);
      shared::event::ui::PanelEvent panel_bg_event(
        shared::event::ui::Action::Open, client::ui::panel::PanelID::PauseBackgroundPanel);
      shared::event::EventBus::Dispatch(panel_bg_event);
      return;
    }
  }

  auto player = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Get(player_id_);
  if (player) {
    auto casted_player = std::static_pointer_cast<shared::core::player::Player>(player);
    casted_player->HandleEvent(event);
  }


  if (network_mediator_) { network_mediator_->AddInputEvent(event); }
}

void GameState::SetPlayerId(const UUIDv4::UUID &player_id)
{
  player_id_ = player_id;

  auto player = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Get(player_id);

  if (player) {
    GCM::Instance().GetContext(GameContextType::Client)->SetLocalPlayer(player);
    if (first_snapshot_received_) {
      shared::event::ShaderEvent shader_event(shared::event::ShaderEventType::ADD);
      shader_event.SetPassType(client::render::shader::PassType::HEALTH);
      shader_event.SetFront(true);
      shared::event::EventBus::GetDispatcher()->Dispatch(shader_event);
      first_snapshot_received_ = false;
    }
  }
}

void GameState::SetEventDispatcher()
{
  key_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::KeyEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });

  projectile_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ProjectileEvent>(
    [this](const shared::event::Event &event) { this->HandleProjectileEvent(event); });
}

void GameState::SetSoundEventDispatcher()
{
  sound_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::SoundEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

void GameState::HandleProjectileEvent(const shared::event::Event &event)
{
  const auto &projectile_event = dynamic_cast<const shared::event::ProjectileEvent &>(event);

  auto projectile = shared::builder::GameObjectBuilder<shared::core::projectile::Projectile>()
                      .Id(projectile_event.GetProjectileId())
                      .AddTransform(projectile_event.GetPosition())
                      .AddMovement(projectile_event.GetDirection())
                      .AddSpeed(projectile_event.GetSpeed())
                      .AddAnimation()
                      .AddProjectileType(projectile_event.GetProjectileType())
                      .NetRole(shared::core::NetRole::SIMULATED)
                      .Build();

  projectile->InitAnimation();

  GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Register(projectile);
}

UUIDv4::UUID GameState::GetPlayerId() const { return player_id_; }

bool GameState::IsPaused() const { return is_paused_; }

void GameState::SetPaused(bool paused) { is_paused_ = paused; }

void GameState::CreatePlayerWithPlayerData(const database::PlayerData &player_data)
{
  auto player = GCM::Instance().GetContext(GameContextType::Client)->GetLocalPlayer();

  if (player) {
    auto casted_player = std::static_pointer_cast<shared::core::player::Player>(player);
    casted_player->LoadPlayerWithPlayerData(player_data);
  }
}

}// namespace chroma::app::states