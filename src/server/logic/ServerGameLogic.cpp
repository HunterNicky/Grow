#include "chroma/server/logic/ServerGameLogic.h"

#include "chroma/shared/builder/GameObjectBuilder.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Fist.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/Javelin.h"
#include "chroma/shared/core/components/Spear.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/packet/events/ProjectileMessage.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/shared/utils/UUID.h"
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

ServerGameLogic::~ServerGameLogic()
{
  GCM::Instance().GetContext(GameContextType::Server)->GetGameObjectManager()->Clear();
}

void ServerGameLogic::Update(const float delta_time)
{
  GCM::Instance()
    .GetContext(GameContextType::Server)
    ->GetGameObjectManager()
    ->ForEach([delta_time](const std::shared_ptr<shared::core::GameObject> &obj) {
      if (obj && obj->HasAuthority()) { obj->OnUpdate(delta_time); }
    });

  if (const auto collision_manager =
        GCM::Instance().GetContext(GameContextType::Client)->GetGameObjectManager()->GetCollisionManager();
    collision_manager) {
    collision_manager->Update();
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
  auto player = chroma::shared::builder::GameObjectBuilder<shared::core::player::Player>()
                  .AddTransform({ 0, 0 })
                  .Id(shared::utils::UUID::Generate())
                  .AddSpeed(50.0F)
                  .AddMovement()
                  .AddAnimation()
                  .AddCamera(shared::render::CameraMode::FollowSmooth, 3.0F, 2.0F, { 64, 128 })
                  .AddColliderBox({ 32.F, 16.F }, { -16.F, 32.F })
                  .AddAudioListener()
                  .AddHealth(100.0F, 1.0F)
                  .AddRun(false, 1.5F)
                  .AddInventory(10)
                  .AddAttack(false)
                  .NetRole(shared::core::NetRole::AUTHORITY)
                  .Build();

  auto javelin = std::make_shared<shared::core::component::Javelin>();
  auto spear = std::make_shared<shared::core::component::Spear>();
  auto fist = std::make_shared<shared::core::component::Fist>();

  player->GetComponent<shared::core::component::Inventory>()->AddInventory(spear);
  player->GetComponent<shared::core::component::Inventory>()->AddInventory(javelin);
  player->GetComponent<shared::core::component::Inventory>()->AddInventory(fist);
  player->SetCurrentWeapon(javelin);

  player->SetupAnimation(player->GetComponent<shared::core::component::SpriteAnimation>());
  GCM::Instance().GetContext(GameContextType::Server)->GetGameObjectManager()->Register(player);

  return player;
}

void ServerGameLogic::HandleInput(shared::event::Event &event, const UUIDv4::UUID &player_id)
{
  const auto player_obj = GCM::Instance().GetContext(GameContextType::Server)->GetGameObjectManager()->Get(player_id);
  if (player_obj && player_obj->HasAuthority()) { player_obj->HandleEvent(event); }
}

std::vector<flatbuffers::Offset<Game::EntityState>> ServerGameLogic::GetEntitiesFlatBuffer(
  flatbuffers::FlatBufferBuilder &builder)
{
  return shared::packet::PacketHandler::GameObjectsToFlatBufferEntities(builder);
}

void ServerGameLogic::OnReceivedProjectileMessage(
  const std::shared_ptr<shared::packet::ProjectileMessage> &projectile_message)
{
  auto event = projectile_message->GetProjectileEvent();
  if (!event) { return; }

  auto projectile = chroma::shared::builder::GameObjectBuilder<shared::core::projectile::Projectile>()
                      .Id(event->GetProjectileId())
                      .AddTransform(event->GetPosition())
                      .AddMovement(event->GetDirection())
                      .AddSpeed(event->GetSpeed())
                      .AddAnimation()
                      .AddProjectileType(event->GetProjectileType())
                      .NetRole(shared::core::NetRole::AUTHORITY)
                      .Build();

  GCM::Instance().GetContext(GameContextType::Server)->GetGameObjectManager()->Register(projectile);
}

}// namespace chroma::server::logic
