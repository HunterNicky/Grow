#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Inventory.h"
#include "chroma/shared/core/components/ProjectileType.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/core/projectile/Projectile.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include "chroma/shared/factory/GameObjectFactory.h"
#include "chroma/shared/packet/adapter/ComponentAdapter.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
#include "chroma/shared/packet/events/ProjectileMessage.h"
#include "chroma/shared/packet/events/SoundEventMessage.h"
#include "common_generated.h"
#include "components_generated.h"
#include "entities_generated.h"
#include "events_generated.h"
#include "messages_generated.h"

#include <cstdint>
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <memory>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

#define UUID_SYSTEM_GENERATOR

namespace chroma::shared::packet {

std::vector<uint8_t> PacketHandler::GameObjectsToFlatBuffer(flatbuffers::FlatBufferBuilder &builder,
  const std::vector<flatbuffers::Offset<Game::EntityState>> &entities,
  const UUIDv4::UUID &player_id,
  const uint64_t time_lapse,
  const uint32_t last_processed_input)
{

  const auto fb_player_id = builder.CreateString(player_id.str());
  const auto fb_entities_vec = builder.CreateVector(entities);
  const auto snapshot = Game::CreateSnapshot(builder, time_lapse, last_processed_input, fb_player_id, fb_entities_vec);
  const auto envelope =
    Game::CreateEnvelope(builder, Game::MsgType::Snapshot, Game::MsgUnion::Snapshot, snapshot.Union());
  builder.Finish(envelope);

  auto buf = builder.Release();

  return { buf.begin(), buf.end() };
}

std::vector<flatbuffers::Offset<Game::EntityState>> PacketHandler::GameObjectsToFlatBufferEntities(
  flatbuffers::FlatBufferBuilder &builder,
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>> &objects)
{
  std::vector<flatbuffers::Offset<Game::EntityState>> fb_entities;

  for (const auto &[uuid, object] : objects) {
    if (!object || !object->HasAuthority()) { continue; }

    std::vector<flatbuffers::Offset<Game::Component>> components;
    adapter::ComponentAdapter::ToComponent(object, builder, components);

    const auto fb_id = builder.CreateString(object->GetId().str());
    const auto fb_type = static_cast<Game::GameObjectType>(object->GetTag());
    const auto fb_components = builder.CreateVector(components);
    auto fb_entity = Game::CreateEntityState(builder, fb_id, fb_type, fb_components);
    fb_entities.push_back(fb_entity);
  }

  return fb_entities;
}


void PacketHandler::UpdateGameObjectWithEntityState(const Game::EntityState *entity_state,
  std::shared_ptr<core::GameObject> &game_object)
{
  if (entity_state == nullptr || game_object == nullptr) { return; }
  game_object->SetId(UUIDv4::UUID::fromStrFactory(entity_state->id()->str()));

  for (const auto &component : *entity_state->components()) {
    if (component == nullptr) { continue; }
    adapter::ComponentAdapter::FromComponent(*component, game_object);
  }

  switch (entity_state->type()) {
  case Game::GameObjectType::Player: {
    const auto player = std::static_pointer_cast<core::player::Player>(game_object);
    if (!player) { break; }

    auto inventory = player->GetComponent<core::component::Inventory>();
    if (inventory && inventory->GetCurrentWeapon()) { player->SetCurrentWeapon(inventory->GetCurrentWeapon()); }
    break;
  }
  case Game::GameObjectType::Projectile: {
    const auto projectile = std::static_pointer_cast<core::projectile::Projectile>(game_object);

    if (!projectile) { break; }

    auto sprite_anim = projectile->GetComponent<core::component::SpriteAnimation>();
    if (sprite_anim == nullptr) { break; }

    if (sprite_anim->GetAnimations().empty()) { projectile->InitAnimation(); }
    break;
  }
  default:
    break;
  }
}

std::vector<uint8_t> PacketHandler::InputMessageToFlatBuffer(const std::shared_ptr<InputMessage> &input_message)
{
  flatbuffers::FlatBufferBuilder builder(1024);

  Game::InputEventType type = Game::InputEventType::None;
  Game::InputEvent event_type = Game::InputEvent::NONE;
  flatbuffers::Offset<void> event_union;

  switch (input_message->GetEventType()) {
  case event::Event::KeyEvent: {
    const auto key_event = std::static_pointer_cast<const event::KeyEvent>(input_message->GetEvent());
    const auto fb_key_event =
      Game::CreateKeyEvent(builder, key_event->GetKey(), key_event->IsPressed(), key_event->IsReleased());
    type = Game::InputEventType::KeyEvent;
    event_type = Game::InputEvent::KeyEvent;
    event_union = fb_key_event.Union();
    break;
  }

  case event::Event::MouseClickEvent: {
    const auto mouse_event = std::static_pointer_cast<const event::MouseEvent>(input_message->GetEvent());
    const auto fb_mouse_pos =
      Game::CreateVec2(builder, mouse_event->GetMousePosition().x, mouse_event->GetMousePosition().y);
    const auto fb_mouse_event = Game::CreateMouseEvent(
      builder, mouse_event->IsLeftButtonPressed(), mouse_event->IsRightButtonPressed(), fb_mouse_pos);
    type = Game::InputEventType::MouseEvent;
    event_type = Game::InputEvent::MouseEvent;
    event_union = fb_mouse_event.Union();
    break;
  }

  default:
    return {};
  }

  const auto fb_input_msg = Game::CreateInputEventMessage(
    builder, input_message->GetSeq(), input_message->GetDeltaTime(), type, event_type, event_union);

  const auto fb_event = Game::CreateEvent(builder, Game::EventUnion::InputEventMessage, fb_input_msg.Union());

  const auto fb_envelope = Game::CreateEnvelope(builder, Game::MsgType::Event, Game::MsgUnion::Event, fb_event.Union());

  builder.Finish(fb_envelope);
  auto buf = builder.Release();

  return { buf.begin(), buf.end() };
}

std::vector<uint8_t> PacketHandler::SoundEventMessageToFlatBuffer(
  const std::shared_ptr<SoundEventMessage> &sound_message)
{
  if (!sound_message) { return {}; }

  flatbuffers::FlatBufferBuilder builder(256);

  const auto fb_sound_id = builder.CreateString(sound_message->GetSoundId());
  const auto fb_source_id = builder.CreateString(sound_message->GetSourceId());
  const auto fb_sound =
    Game::CreateSoundEvent(builder, fb_sound_id, sound_message->GetVolume(), sound_message->GetPitch(), fb_source_id);
  const auto fb_sound_msg =
    Game::CreateSoundEventMessage(builder, sound_message->GetSeq(), sound_message->GetDeltaTime(), fb_sound);

  const auto fb_event = Game::CreateEvent(builder, Game::EventUnion::SoundEventMessage, fb_sound_msg.Union());
  const auto fb_envelope = Game::CreateEnvelope(builder, Game::MsgType::Event, Game::MsgUnion::Event, fb_event.Union());

  builder.Finish(fb_envelope);
  auto buf = builder.Release();
  return { buf.begin(), buf.end() };
}

UUIDv4::UUID PacketHandler::SnapshotGetUUID(const Game::Snapshot *snapshot)
{
  if (snapshot == nullptr || snapshot->player_id() == nullptr) { return UUIDv4::UUID{}; }
  return UUIDv4::UUID::fromStrFactory(snapshot->player_id()->str());
}

uint32_t PacketHandler::SnapshotGetLastProcessedInputSeq(const Game::Snapshot *snapshot)
{
  if (snapshot == nullptr) { return 0; }
  return snapshot->last_processed_input();
}

uint64_t PacketHandler::SnapshotGetTimeLapse(const Game::Snapshot *snapshot)
{
  if (snapshot == nullptr) { return 0; }
  return snapshot->server_time();
}

void PacketHandler::SnapshotToGameObjects(const Game::Snapshot *snapshot,
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>> &game_objects)
{
  if (snapshot == nullptr || snapshot->entities() == nullptr) { return; }

  const std::string local_player_id_str =
    snapshot->player_id() != nullptr ? snapshot->player_id()->str() : std::string{};

  for (const auto &entity_state : *snapshot->entities()) {
    if (entity_state == nullptr || entity_state->id() == nullptr) { continue; }

    const UUIDv4::UUID entity_id = UUIDv4::UUID::fromStrFactory(entity_state->id()->str());
    const bool is_local_player = (entity_state->id()->str() == local_player_id_str);

    std::shared_ptr<core::GameObject> game_object;
    if (const auto it = game_objects.find(entity_id); it != game_objects.end()) {
      game_object = it->second;
    } else {
      game_object = factory::GameObjectFactory::Create(entity_state, is_local_player);
      if (game_object) { game_objects.emplace(entity_id, game_object); }
    }

    if (!game_object) { continue; }

    if (entity_state->type() == Game::GameObjectType::Player) {
      if (const auto player = std::dynamic_pointer_cast<core::player::Player>(game_object)) {
        player->SetNetRole(is_local_player ? core::NetRole::AUTONOMOUS : core::NetRole::SIMULATED);
      }
    }

    UpdateGameObjectWithEntityState(entity_state, game_object);
  }
}

std::shared_ptr<InputMessage> PacketHandler::EventToInputMessage(const Game::Event *evt)
{
  if (evt == nullptr) { return nullptr; }
  if (evt->event_type() != Game::EventUnion::InputEventMessage) { return nullptr; }

  const auto *input_msg = evt->event_as<Game::InputEventMessage>();
  if (input_msg == nullptr) { return nullptr; }

  std::shared_ptr input_message = std::make_unique<InputMessage>();
  input_message->SetSeq(input_msg->seq());
  input_message->SetDeltaTime(input_msg->dt());

  switch (input_msg->event_type()) {
  case Game::InputEvent::KeyEvent: {
    const auto *fb_key_event = input_msg->event_as_KeyEvent();
    if (fb_key_event != nullptr) {
      auto key_event = event::KeyEvent(fb_key_event->key());
      key_event.SetPressed(fb_key_event->is_pressed());
      input_message->SetEventType(event::Event::Type::KeyEvent);
      input_message->SetEvent(key_event);
    }
    break;
  }
  case Game::InputEvent::MouseEvent: {
    const auto *fb_mouse_event = input_msg->event_as_MouseEvent();
    if (fb_mouse_event != nullptr) {
      const Vector2 position{ fb_mouse_event->mouse_position()->x(), fb_mouse_event->mouse_position()->y() };
      const auto mouse_event = event::MouseEvent(position, fb_mouse_event->left(), fb_mouse_event->right());
      input_message->SetEventType(event::Event::Type::MouseClickEvent);
      input_message->SetEvent(mouse_event);
    }
    break;
  }
  default:
    break;
  }
  return input_message;
}

std::vector<uint8_t> PacketHandler::ProjectileMessageToFlatBuffer(
  const std::shared_ptr<ProjectileMessage> &projectile_message)
{
  if (!projectile_message) { return {}; }

  auto &evt = projectile_message->GetProjectileEvent();
  if (!evt) { return {}; }

  flatbuffers::FlatBufferBuilder builder(256);

  const auto fb_id = builder.CreateString(evt->GetProjectileId().str());

  const auto fb_dir = Game::CreateVec2(builder, evt->GetDirection().x, evt->GetDirection().y);

  const auto fb_position = Game::CreateVec2(builder, evt->GetPosition().x, evt->GetPosition().y);

  const auto fb_proj_event = Game::CreateProjectileEvent(
    builder, fb_id, static_cast<Game::TypeProjectile>(evt->GetProjectileType()), fb_position, fb_dir, evt->GetSpeed());

  const auto fb_proj_msg = Game::CreateProjectileEventMessage(
    builder, projectile_message->GetSeq(), projectile_message->GetDeltaTime(), fb_proj_event);

  const auto fb_event = Game::CreateEvent(builder, Game::EventUnion::ProjectileEventMessage, fb_proj_msg.Union());

  const auto fb_envelope = Game::CreateEnvelope(builder, Game::MsgType::Event, Game::MsgUnion::Event, fb_event.Union());

  builder.Finish(fb_envelope);

  auto buf = builder.Release();
  return { buf.begin(), buf.end() };
}

std::shared_ptr<ProjectileMessage> PacketHandler::EventToProjectileMessage(const Game::Event *evt)
{
  if (evt == nullptr) { return nullptr; }
  if (evt->event_type() != Game::EventUnion::ProjectileEventMessage) { return nullptr; }

  const auto *proj_msg = evt->event_as<Game::ProjectileEventMessage>();
  if (proj_msg == nullptr) { return nullptr; }

  std::shared_ptr<ProjectileMessage> projectile_message = std::make_unique<ProjectileMessage>();
  projectile_message->SetSeq(proj_msg->seq());
  projectile_message->SetDeltaTime(proj_msg->dt());

  const auto *fb_proj_event = proj_msg->event();
  if (fb_proj_event != nullptr) {

    const UUIDv4::UUID projectile_id = UUIDv4::UUID::fromStrFactory(fb_proj_event->projectile_id()->str());

    const Vector2 direction{ fb_proj_event->direction()->x(), fb_proj_event->direction()->y() };

    const Vector2 position{ fb_proj_event->position()->x(), fb_proj_event->position()->y() };

    auto projectile_event = std::make_shared<event::ProjectileEvent>(
      static_cast<core::component::TypeProjectile>(fb_proj_event->type()), direction, fb_proj_event->speed());
    projectile_event->SetProjectileId(projectile_id);
    projectile_event->SetPosition(position);

    projectile_message->GetProjectileEvent() = projectile_event;
  }

  return projectile_message;
}
}// namespace chroma::shared::packet
