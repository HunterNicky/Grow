#include "chroma/app/states/GameState.h"

#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/KeyEvent.h"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <uuid_v4.h>

namespace chroma::app::states {

GameState::GameState()
  : State("GameState"),
    game_objects_(
      std::make_shared<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>()),
    network_mediator_(nullptr)
{
  auto player = std::make_shared<shared::core::player::Player>();
  SetPlayerId(player->GetId());
  player->SetNetRole(shared::core::NetRole::ROLE_AutonomousProxy);
  player->InitComponents();
  player_id_ = player->GetId();
  game_objects_->emplace(player->GetId(), player);
}

GameState::GameState(std::shared_ptr<GameNetworkMediator> network_mediator)
  : State("GameState"),
    game_objects_(
      std::make_shared<std::unordered_map<UUIDv4::UUID, std::shared_ptr<shared::core::GameObject>>>()),
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

void GameState::SetPlayerId(const UUIDv4::UUID &player_id) { player_id_ = player_id; }

void GameState::SetEventDispatcher()
{
  shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::KeyEvent>([this](shared::event::Event &event) { this->OnEvent(event); });
}

std::shared_ptr<shared::core::player::Player> GameState::GetPlayer() const
{
  auto it = game_objects_->find(player_id_);
  if (it != game_objects_->end()) {
    return std::static_pointer_cast<shared::core::player::Player>(it->second);
  }
  return nullptr;
}

UUIDv4::UUID GameState::GetPlayerId() const { return player_id_; }

}// namespace chroma::app::states