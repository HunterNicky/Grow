#include "chroma/server/logic/ServerGameLogic.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Spear.h"
#include "chroma/shared/core/components/Javelin.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/builder/GameObjectBuilder.h"
#include "entities_generated.h"

#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <memory>
#include <uuid_v4.h>
#include <vector>

namespace chroma::server::logic {

ServerGameLogic::ServerGameLogic() = default;

void ServerGameLogic::CreateWorld()
{
  // Criar mundo do jogo
}

ServerGameLogic::~ServerGameLogic() { game_objects_.clear(); }

void ServerGameLogic::Update(const float delta_time) const
{
  for (const auto &[id, obj] : game_objects_) {
    if (obj && obj->HasAuthority()) { obj->OnUpdate(delta_time); }
  }
}

void ServerGameLogic::OnReceivedInputMessage(const std::shared_ptr<shared::packet::InputMessage> &input_message,
  const UUIDv4::UUID &player_id)
{
  const std::shared_ptr<shared::event::Event> event = input_message->GetEvent();
  HandleInput(*event, player_id);
}

std::shared_ptr<shared::core::player::Player> ServerGameLogic::CreatePlayer()
{
  auto player = chroma::shared::builder::GameObjectBuilder<chroma::shared::core::player::Player>()
    .AddTransform({0,0})
    .AddSpeed(50.0F)
    .AddMovement()
    .AddAnimation()
    .AddCamera(chroma::shared::render::CameraMode::FollowSmooth, 3.0F, 2.0F, {64,128})
    .AddAudioListener()
    .AddHealth(100.0F, 1.0F)
    .AddRun(false, 1.5F)
    .AddInventory(10)
    .AddAttack(false)
    .NetRole(shared::core::NetRole::AUTHORITY)
    .Build();

  auto javelin = std::make_shared<chroma::shared::core::component::Javelin>();
  auto spear = std::make_shared<chroma::shared::core::component::Spear>();

  player->GetComponent<shared::core::component::Inventory>()->AddInventory(spear);
  player->GetComponent<shared::core::component::Inventory>()->AddInventory(javelin);
  player->SetCurrentWeapon(javelin);

  player->SetupAnimation(player->GetComponent<shared::core::component::SpriteAnimation>());

  game_objects_.emplace(player->GetId(), player);
  return player;
}

void ServerGameLogic::HandleInput(const shared::event::Event &event, const UUIDv4::UUID &player_id)
{
  auto it = game_objects_.find(player_id);
  if (it != game_objects_.end()) {
    auto player = std::static_pointer_cast<shared::core::player::Player>(it->second);
    if (player && player->HasAuthority()) { player->HandleEvent(event); }
  }
}

std::vector<flatbuffers::Offset<Game::EntityState>> ServerGameLogic::GetEntitiesFlatBuffer(
  flatbuffers::FlatBufferBuilder &builder) const
{
  return shared::packet::PacketHandler::GameObjectsToFlatBufferEntities(builder, game_objects_);
}

void ServerGameLogic::OnReceivedProjectileMessage(const std::shared_ptr<shared::packet::ProjectileMessage> &projectile_message)
{
    auto event = projectile_message->GetProjectileEvent();
    if (!event) {
        return;
    }

    auto projectile = chroma::shared::builder::GameObjectBuilder<chroma::shared::core::projectile::Projectile>()
    .Id(event->GetProjectileId())
    .AddTransform(event->GetPosition())
    .AddMovement(event->GetDirection())
    .AddSpeed(event->GetSpeed())
    .AddAnimation()
    .AddProjectileType(event->GetProjectileType())
    .NetRole(shared::core::NetRole::AUTHORITY)
    .Build();
    
    game_objects_.emplace(projectile->GetId(), projectile);
}

}// namespace chroma::server::logic
