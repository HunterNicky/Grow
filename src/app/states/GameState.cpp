#include "chroma/app/states/GameState.h"

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/client/render/shader/shaders/HealthPass.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <uuid_v4.h>

namespace chroma::app::states {

GameState::GameState()
  : State("GameState"),
    game_objects_(std::make_shared<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>()),
    network_mediator_(nullptr)
{
  auto player = shared::builder::GameObjectBuilder<shared::core::player::Player>()
        .AddTransform({0,0})
        .AddSpeed(50.0F)
        .AddMovement()
        .AddAnimation()
        .AddCamera(shared::render::CameraMode::FollowSmooth, 3.0F, 2.0F, {64,128})
        .AddAudioListener()
        .AddHealth(100.0F, 1.0F)
        .AddRun(false, 1.5F)
        .NetRole(shared::core::NetRole::ROLE_AutonomousProxy)
        .Build();

  SetPlayerId(player->GetId());
  player->SetupAnimation(player->GetComponent<shared::core::component::SpriteAnimation>());
  game_objects_->emplace(player->GetId(), player);
}

GameState::GameState(std::shared_ptr<GameNetworkMediator> network_mediator)
  : State("GameState"),
    game_objects_(std::make_shared<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>()),
    network_mediator_(std::move(network_mediator))
{
  network_mediator_->SetGameObjects(game_objects_);
}


GameState::~GameState() { game_objects_->clear(); }

void GameState::OnRender()
{

  if (!IsActive() || game_objects_->empty()) { return; }

  for (const auto &[uuid, obj] : *game_objects_) {
    if (obj && obj->IsActive()) { obj->OnRender(); }
  }
}

void GameState::OnUpdate(float delta_time)
{
  if (!IsActive()) { return; }

  if (network_mediator_) { network_mediator_->UpdateInterpolation(static_cast<uint64_t>(delta_time * 1000)); }

  for (const auto &[uuid, obj] : *game_objects_) {
    if (obj && obj->IsActive()) { obj->OnUpdate(delta_time); }
  }
}

void GameState::SetGameObjects(
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>> &game_objects)
{
  game_objects_ =
    std::make_shared<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>(game_objects);
}

std::shared_ptr<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>> &
  GameState::GetGameObjects()
{
  return game_objects_;
}

void GameState::OnEvent(shared::event::Event &event)
{
  if (player_id_ == UUIDv4::UUID{}) { return; }

  auto it = game_objects_->find(player_id_);
  if (it != game_objects_->end()) {
    auto player = std::static_pointer_cast<shared::core::player::Player>(it->second);
    player->HandleEvent(event);
  }

  if (network_mediator_) { network_mediator_->AddInputEvent(event); }
}

void GameState::SetPlayerId(const UUIDv4::UUID &player_id) {
   player_id_ = player_id; 

   auto player = GetPlayer();

   if(player)
   {
       shared::context::GameContext::GetInstance().SetLocalPlayer(player);
      if(first_snapshot_received_) 
      {
          auto health_pass = std::make_unique<client::render::shader::shaders::HealthPass>();
          render_mediator_->AddShaderFront(std::move(health_pass));
          first_snapshot_received_ = false;
      }
   }
}

void GameState::SetEventDispatcher()
{
  shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::KeyEvent>(
    [this](shared::event::Event &event) { this->OnEvent(event); });
}

std::shared_ptr<shared::core::player::Player> GameState::GetPlayer() const
{
  auto it = game_objects_->find(player_id_);
  if (it != game_objects_->end()) { return std::static_pointer_cast<shared::core::player::Player>(it->second); }
  return nullptr;
}

UUIDv4::UUID GameState::GetPlayerId() const { return player_id_; }

void GameState::SetRenderMediator(std::shared_ptr<mediator::RenderMediator> mediator)
{
  render_mediator_ = std::move(mediator);
}

}// namespace chroma::app::states