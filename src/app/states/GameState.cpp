#include "chroma/app/states/GameState.h"

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/mediator/RenderMediator.h"
#include "chroma/client/render/shader/shaders/HealthPass.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/GameObjectManager.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/core/world/World.h"
#include "chroma/shared/core/world/WorldSystem.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/ProjectileEvent.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/utils/UUID.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <uuid_v4.h>

namespace chroma::app::states {

GameState::GameState() : State("GameState"), network_mediator_(nullptr)
{
}

GameState::GameState(std::shared_ptr<GameNetworkMediator> network_mediator)
  : State("GameState"), network_mediator_(std::move(network_mediator))
{
}

GameState::~GameState() { GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Clear(); }

void GameState::OnRender()
{
  if (!IsActive()) { return; }
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

  auto player = GetPlayer();

  if (player) {
    GCM::Instance().GetContext(GameContextType::Client)->SetLocalPlayer(player);
    if (first_snapshot_received_) {
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

  shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ProjectileEvent>(
    [this](const shared::event::Event &event) { this->HandleProjectileEvent(event); });
}

void GameState::HandleProjectileEvent(const shared::event::Event &event) const
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

std::shared_ptr<shared::core::player::Player> GameState::GetPlayer() const
{
  auto player = GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->Get(player_id_);
  if (player) { return std::static_pointer_cast<shared::core::player::Player>(player); }
  return nullptr;
}

UUIDv4::UUID GameState::GetPlayerId() const { return player_id_; }

void GameState::SetRenderMediator(std::shared_ptr<mediator::RenderMediator> mediator)
{
  render_mediator_ = std::move(mediator);
}

}// namespace chroma::app::states