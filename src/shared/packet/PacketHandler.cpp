#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include "chroma/shared/factory/GameObjectFactory.h"
#include "chroma/shared/packet/events/InputEventMessage.h"
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

    auto pos = object->GetComponent<core::component::Transform>();
    auto vel = object->GetComponent<core::component::Speed>();
    auto mov = object->GetComponent<core::component::Movement>();
    auto color = object->GetComponent<core::component::Coloring>();

    std::vector<flatbuffers::Offset<Game::Component>> components;

    if (pos) {
      const auto vec2 = Game::CreateVec2(builder, pos->GetPosition().x, pos->GetPosition().y);
      auto fb_pos = Game::CreatePosition(builder, vec2);
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Position, fb_pos.Union()));
    }

    if (vel) {
      const auto vec2 = Game::CreateVec2(builder, vel->GetSpeed().x, vel->GetSpeed().y);
      auto fb_vel = Game::CreateVelocity(builder, vec2);
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Velocity, fb_vel.Union()));
    }

    if (color) {
      auto fb_color = Game::CreateColor(builder,
        static_cast<int8_t>(color->GetRed()),
        static_cast<int8_t>(color->GetGreen()),
        static_cast<int8_t>(color->GetBlue()),
        static_cast<int8_t>(color->GetAlpha()));
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Color, fb_color.Union()));
    }

    if (color) {
      auto fb_color = Game::CreateColor(builder,
        static_cast<int8_t>(color->GetRed()),
        static_cast<int8_t>(color->GetGreen()),
        static_cast<int8_t>(color->GetBlue()),
        static_cast<int8_t>(color->GetAlpha()));
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Color, fb_color.Union()));
    }

    if (mov) {
      const auto vec2 = Game::CreateVec2(builder, mov->GetDirection().x, mov->GetDirection().y);
      auto fb_mov = Game::CreateMovement(builder, vec2);
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Movement, fb_mov.Union()));
    }

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
  switch (entity_state->type()) {
  case Game::GameObjectType::Player: {
    const auto player = std::static_pointer_cast<core::player::Player>(game_object);
    if (player) {
      player->SetId(UUIDv4::UUID(entity_state->id()->str()));

      for (const auto &component : *entity_state->components()) {
        switch (component->type_type()) {
        case Game::ComponentUnion::Position: {
          ComponentToTransform(component, game_object);
          break;
        }
        case Game::ComponentUnion::Velocity: {
          ComponentToSpeed(component, game_object);
          break;
        }

        case Game::ComponentUnion::Movement: {
          ComponentToMovement(component, game_object);
          const auto movement = game_object->GetComponent<core::component::Movement>();
          player->UpdateAnimationFromDirection(movement->GetDirection());
          break;
        }

        case Game::ComponentUnion::Color: {
          ComponentToColor(component, game_object);
          break;
        }

        default:
          break;
        }
      }
    }
    break;
  }
  default:
    break;
  }
}

void PacketHandler::ComponentToSpeed(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_vel = component->type_as_Velocity();
  if (fb_vel != nullptr) {
    const auto *vel_vec = fb_vel->vel();
    const auto speed = game_object->GetComponent<core::component::Speed>();
    if (speed) {
      speed->SetSpeed(Vector2{ vel_vec->x(), vel_vec->y() });
      game_object->AttachComponent(speed);
    }
  }
}

void PacketHandler::ComponentToColor(const Game::Component *component, std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_color = component->type_as_Color();
  if (fb_color != nullptr) {
    const auto color = game_object->GetComponent<core::component::Coloring>();
    if (color) {
      color->SetColoring(fb_color->r(), fb_color->g(), fb_color->b(), fb_color->a());
      game_object->AttachComponent(color);
    }
  }
}

void PacketHandler::ComponentToMovement(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_mov = component->type_as_Movement();
  if (fb_mov != nullptr) {
    const auto *mov_vec = fb_mov->mov();
    const auto movement = game_object->GetComponent<core::component::Movement>();
    if (movement) {
      movement->SetDirection(Vector2{ mov_vec->x(), mov_vec->y() });
      game_object->AttachComponent(movement);
    }
  }
}

void PacketHandler::ComponentToTransform(const Game::Component *component,
  const std::shared_ptr<core::GameObject> &game_object)
{
  const auto *fb_pos = component->type_as_Position();
  if (fb_pos != nullptr) {
    const auto *pos_vec = fb_pos->pos();
    const auto transform = game_object->GetComponent<core::component::Transform>();
    if (transform) {
      transform->SetPosition(Vector2{ pos_vec->x(), pos_vec->y() });
      game_object->AttachComponent(transform);
    }
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
  return UUIDv4::UUID(snapshot->player_id()->str());
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

    const UUIDv4::UUID entity_id(entity_state->id()->str());
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
        player->SetNetRole(is_local_player ? core::NetRole::ROLE_AutonomousProxy : core::NetRole::ROLE_SimulatedProxy);
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

}// namespace chroma::shared::packet
