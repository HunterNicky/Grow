#include "chroma/server/logic/ServerGameLogic.h"

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
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
  auto player = std::make_shared<shared::core::player::Player>();
  player->SetNetRole(shared::core::NetRole::ROLE_Authority);
  player->InitComponents();
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

}// namespace chroma::server::logic
