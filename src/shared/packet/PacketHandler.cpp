#include "chroma/shared/packet/PacketHandler.h"
#include "GameObject_generated.h"
#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Movement.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/KeyEvent.h"
#include "chroma/shared/events/MouseEvent.h"
#include "chroma/shared/packet/InputMessage.h"

#include <cstddef>
#include <cstdint>
#include <flatbuffers/buffer.h>
#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/verifier.h>
#include <memory>
#include <raylib.h>
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

  auto fb_player_id = builder.CreateString(player_id.str());
  auto fb_entities_vec = builder.CreateVector(entities);
  auto snapshot = Game::CreateSnapshot(builder, time_lapse, last_processed_input, fb_player_id, fb_entities_vec);
  auto envelope = Game::CreateEnvelope(builder, Game::MsgType::SNAPSHOT, Game::MsgUnion::Snapshot, snapshot.Union());
  builder.Finish(envelope);

  auto buf = builder.Release();

  return { buf.begin(), buf.end() };
}

std::vector<flatbuffers::Offset<Game::EntityState>> PacketHandler::GameObjectsToFlatBufferEntities(
  flatbuffers::FlatBufferBuilder &builder,
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> &objects)
{
  std::vector<flatbuffers::Offset<Game::EntityState>> fb_entities;

  for (const auto &[uuid, object] : objects) {
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

  auto fb_player_id = builder.CreateString(player_id.str());
  auto fb_entities_vec = builder.CreateVector(fb_entities);
  auto snapshot = Game::CreateSnapshot(builder, time_lapse, last_processed_input, fb_player_id, fb_entities_vec);
  auto envelope = Game::CreateEnvelope(builder, Game::MsgType::SNAPSHOT, Game::MsgUnion::Snapshot, snapshot.Union());
  builder.Finish(envelope);

  auto buf = builder.Release();

  return { buf.begin(), buf.end() };
}

void PacketHandler::FlatBufferToGameObject(const uint8_t *data,
  const size_t size,
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<core::GameObject>> &game_objects)
{

  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::SNAPSHOT) { return; }

  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  if (snapshot == nullptr) { return; }

  for (const auto &entity_state : *snapshot->entities()) {
    auto it = game_objects.find(UUIDv4::UUID(entity_state->id()->str()));

    if (it != game_objects.end()) {
      UpdateGameObjectWithEntityState(entity_state, it->second);
    } else {

      std::shared_ptr<core::GameObject> game_object = nullptr;
      switch (entity_state->type()) {
      case Game::GameObjectType::PLAYER: {
        const std::shared_ptr<core::player::Player> player =
          std::make_shared<core::player::Player>();
        player->InitComponents();
        player->SetId(UUIDv4::UUID(entity_state->id()->str()));
        game_object = player;
        game_objects.emplace(player->GetId(), player);
        break;
      }
      }
      UpdateGameObjectWithEntityState(entity_state, game_object);
    }
  }
}

void PacketHandler::UpdateGameObjectWithEntityState(const Game::EntityState *entity_state,
  std::shared_ptr<core::GameObject> &game_object)
{
  switch (entity_state->type()) {
  case Game::GameObjectType::PLAYER: {
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

void PacketHandler::ComponentToColor(const Game::Component *component,
  std::shared_ptr<chroma::shared::core::GameObject> &game_object)
{
  const auto *fb_color = component->type_as_Color();
  if (fb_color != nullptr) {
    auto color = game_object->GetComponent<chroma::shared::core::component::Coloring>();
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

std::shared_ptr<InputMessage> PacketHandler::FlatBufferToInputMessage(const uint8_t *data, const std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return nullptr; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::INPUT) { return nullptr; }

  const auto *input_msg = envelope->msg_as<Game::InputMessage>();

  if (input_msg == nullptr) { return nullptr; }

  std::shared_ptr<InputMessage> input_message = std::make_unique<InputMessage>();

  input_message->SetSeq(input_msg->seq());
  input_message->SetDeltaTime(input_msg->dt());

  if (input_msg->event_type() == Game::InputEvent::KeyEvent) {
    const auto *fb_key_event = input_msg->event_as_KeyEvent();
    if (fb_key_event != nullptr) {
      auto key_event = event::KeyEvent(fb_key_event->key());
      key_event.SetPressed(fb_key_event->is_pressed());
      input_message->SetEventType(event::Event::Type::KeyEvent);
      input_message->SetEvent(key_event);
    }
  } else if (input_msg->event_type() == Game::InputEvent::MouseEvent) {
    const auto *fb_mouse_event = input_msg->event_as_MouseEvent();
    if (fb_mouse_event != nullptr) {
      const Vector2 position{ fb_mouse_event->mouse_position()->x(), fb_mouse_event->mouse_position()->y() };
      const auto mouse_event = event::MouseEvent(position, fb_mouse_event->left(), fb_mouse_event->right());
      input_message->SetEventType(event::Event::Type::MouseEvent);
      input_message->SetEvent(mouse_event);
    }
  }

  return input_message;
}

std::vector<uint8_t> PacketHandler::InputMessageToFlatBuffer(
  const std::shared_ptr<InputMessage> &input_message)
{
  flatbuffers::FlatBufferBuilder builder(1024);

  Game::InputEventType type = Game::InputEventType::NONE;
  Game::InputEvent event_type = Game::InputEvent::NONE;
  flatbuffers::Offset<void> event_union;

  switch (input_message->GetEventType()) {
  case event::Event::KeyEvent: {
    const auto key_event = std::static_pointer_cast<const event::KeyEvent>(input_message->GetEvent());
    const auto fb_key_event =
      Game::CreateKeyEvent(builder, key_event->GetKey(), key_event->IsPressed(), key_event->IsReleased());
    type = Game::InputEventType::KEYEVENT;
    event_type = Game::InputEvent::KeyEvent;
    event_union = fb_key_event.Union();
    break;
  }

  case event::Event::MouseClickEvent: {
    const auto mouse_event = std::static_pointer_cast<const event::MouseEvent>(input_message->GetEvent());
    const auto fb_mouse_pos = Game::CreateVec2(builder, mouse_event->GetMousePosition().x, mouse_event->GetMousePosition().y);
    const auto fb_mouse_event = Game::CreateMouseEvent(
      builder, mouse_event->IsLeftButtonPressed(), mouse_event->IsRightButtonPressed(), fb_mouse_pos);
    type = Game::InputEventType::MOUSEEVENT;
    event_type = Game::InputEvent::MouseEvent;
    event_union = fb_mouse_event.Union();
    break;
  }

  default:
    return {};
  }

  const auto fb_input_msg = Game::CreateInputMessage(builder, 0, 0.0F, type, event_type, event_union);

  const auto fb_envelope =
    Game::CreateEnvelope(builder, Game::MsgType::INPUT, Game::MsgUnion::InputMessage, fb_input_msg.Union());

  builder.Finish(fb_envelope);
  auto buf = builder.Release();

  return { buf.begin(), buf.end() };
}

UUIDv4::UUID PacketHandler::FlatBufferSnapshotGetUUID(const uint8_t *data, const std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return UUIDv4::UUID{}; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::SNAPSHOT) { return UUIDv4::UUID{}; }

  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  if (snapshot == nullptr) { return UUIDv4::UUID{}; }

  return UUIDv4::UUID(snapshot->player_id()->str());
}

uint32_t PacketHandler::FlatBufferSnapshotGetLastProcessedInputSeq(const uint8_t *data, std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return 0; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::SNAPSHOT) { return 0; }

  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  if (snapshot == nullptr) { return 0; }

  return snapshot->last_processed_input();
}

uint32_t PacketHandler::FlatBufferInputMessageGetSequenceNumber(const uint8_t *data, std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return 0; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::INPUT) { return 0; }

  const auto *input_msg = envelope->msg_as<Game::InputMessage>();
  if (input_msg == nullptr) { return 0; }

  return input_msg->seq();
}

uint64_t PacketHandler::FlatBufferSnapshotGetTimeLapse(const uint8_t *data, std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return 0; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::SNAPSHOT) { return 0; }

  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  if (snapshot == nullptr) { return 0; }

  return snapshot->server_time();
}

}// namespace chroma::shared::packet
