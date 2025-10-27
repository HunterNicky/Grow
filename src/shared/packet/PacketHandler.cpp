#include "chroma/shared/packet/PacketHandler.h"
#include "GameObject_generated.h"
#include "chroma/shared/core/GameObject.h"
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
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/verifier.h>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#include <uuid_v4.h>
#include <vector>

namespace chroma::shared::packet {

std::vector<uint8_t> PacketHandler::GameObjectsToFlatBuffer(
  const std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> &objects,
  const UUIDv4::UUID &player_id,
  uint64_t time_lapse,
  uint32_t last_processed_input)
{
  flatbuffers::FlatBufferBuilder builder(1024);

  std::vector<flatbuffers::Offset<Game::EntityState>> fb_entities;

  for (const auto &[uuid, object] : objects) {
    auto pos = object->GetComponent<chroma::shared::core::component::Transform>();
    auto vel = object->GetComponent<chroma::shared::core::component::Speed>();

    std::vector<flatbuffers::Offset<Game::Component>> components;

    if (pos) {
      auto vec2 = Game::CreateVec2(builder, pos->GetPosition().x, pos->GetPosition().y);
      auto fb_pos = Game::CreatePosition(builder, vec2);
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Position, fb_pos.Union()));
    }

    if (vel) {
      auto vec2 = Game::CreateVec2(builder, vel->GetSpeed().x, vel->GetSpeed().y);
      auto fb_vel = Game::CreateVelocity(builder, vec2);
      components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Velocity, fb_vel.Union()));
    }

    auto fb_id = builder.CreateString(object->GetId().str());
    auto fb_type = static_cast<Game::GameObjectType>(object->GetTag());
    auto fb_components = builder.CreateVector(components);
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
  size_t size,
  std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> &game_objects)
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

      std::shared_ptr<chroma::shared::core::GameObject> game_object = nullptr;
      switch (entity_state->type()) {
      case Game::GameObjectType::PLAYER: {
        const std::shared_ptr<chroma::shared::core::player::Player> player =
          std::make_shared<chroma::shared::core::player::Player>();
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
  std::shared_ptr<chroma::shared::core::GameObject> &game_object)
{
  switch (entity_state->type()) {
  case Game::GameObjectType::PLAYER: {
    auto player = std::dynamic_pointer_cast<chroma::shared::core::player::Player>(game_object);
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
  std::shared_ptr<chroma::shared::core::GameObject> &game_object)
{
  const auto *fb_vel = component->type_as_Velocity();
  if (fb_vel != nullptr) {
    const auto *vel_vec = fb_vel->vel();
    auto speed = game_object->GetComponent<chroma::shared::core::component::Speed>();
    if (speed) {
      speed->SetSpeed(Vector2{ vel_vec->x(), vel_vec->y() });
      game_object->AttachComponent(speed);
    }
  }
}

void PacketHandler::ComponentToMovement(const Game::Component *component,
  std::shared_ptr<chroma::shared::core::GameObject> &game_object)
{
  const auto *fb_mov = component->type_as_Movement();
  if (fb_mov != nullptr) {
    const auto *mov_vec = fb_mov->mov();
    auto movement = game_object->GetComponent<chroma::shared::core::component::Movement>();
    if (movement) {
      movement->SetDirection(Vector2{ mov_vec->x(), mov_vec->y() });
      game_object->AttachComponent(movement);
    }
  }
}

void PacketHandler::ComponentToTransform(const Game::Component *component,
  std::shared_ptr<chroma::shared::core::GameObject> &game_object)
{
  const auto *fb_pos = component->type_as_Position();
  if (fb_pos != nullptr) {
    const auto *pos_vec = fb_pos->pos();
    auto transform = game_object->GetComponent<chroma::shared::core::component::Transform>();
    if (transform) {
      transform->SetPosition(Vector2{ pos_vec->x(), pos_vec->y() });
      game_object->AttachComponent(transform);
    }
  }
}

std::shared_ptr<shared::packet::InputMessage> PacketHandler::FlatBufferToInputMessage(const uint8_t *data,
  std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return nullptr; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::INPUT) { return nullptr; }

  const auto *input_msg = envelope->msg_as<Game::InputMessage>();

  if (input_msg == nullptr) { return nullptr; }

  std::shared_ptr<shared::packet::InputMessage> input_message = std::make_unique<shared::packet::InputMessage>();

  input_message->SetSeq(input_msg->seq());
  input_message->SetDeltaTime(input_msg->dt());

  if (input_msg->event_type() == Game::InputEvent::KeyEvent) {
    shared::event::KeyEvent key_event;
    const auto *fb_key_event = input_msg->event_as_KeyEvent();
    if (fb_key_event != nullptr) {
      key_event = shared::event::KeyEvent(fb_key_event->key());
      key_event.SetPressed(fb_key_event->is_pressed());
      input_message->SetEventType(shared::event::Event::Type::KeyEvent);
      input_message->SetEvent(key_event);
    }
  } else if (input_msg->event_type() == Game::InputEvent::MouseEvent) {
    shared::event::MouseEvent mouse_event;
    const auto *fb_mouse_event = input_msg->event_as_MouseEvent();
    if (fb_mouse_event != nullptr) {
      Vector2 position{ fb_mouse_event->mouse_position()->x(), fb_mouse_event->mouse_position()->y() };
      mouse_event = shared::event::MouseEvent(position, fb_mouse_event->left(), fb_mouse_event->right());
      input_message->SetEventType(shared::event::Event::Type::MouseEvent);
      input_message->SetEvent(mouse_event);
    }
  }

  return input_message;
}

std::vector<uint8_t> PacketHandler::InputMessageToFlatBuffer(
  const std::shared_ptr<shared::packet::InputMessage> &input_message)
{
  flatbuffers::FlatBufferBuilder builder(1024);

  Game::InputEventType type = Game::InputEventType::NONE;
  Game::InputEvent event_type = Game::InputEvent::NONE;
  flatbuffers::Offset<void> event_union;

  switch (input_message->GetEventType()) {
  case shared::event::Event::KeyEvent: {
    const auto key_event = std::static_pointer_cast<const shared::event::KeyEvent>(input_message->GetEvent());
    auto fb_key_event =
      Game::CreateKeyEvent(builder, key_event->GetKey(), key_event->IsPressed(), key_event->IsReleased());
    type = Game::InputEventType::KEYEVENT;
    event_type = Game::InputEvent::KeyEvent;
    event_union = fb_key_event.Union();
    break;
  }

  case shared::event::Event::MouseClickEvent: {
    const auto mouse_event = std::static_pointer_cast<const shared::event::MouseEvent>(input_message->GetEvent());
    auto fb_mouse_pos = Game::CreateVec2(builder, mouse_event->GetMousePosition().x, mouse_event->GetMousePosition().y);
    auto fb_mouse_event = Game::CreateMouseEvent(
      builder, mouse_event->IsLeftButtonPressed(), mouse_event->IsRightButtonPressed(), fb_mouse_pos);
    type = Game::InputEventType::MOUSEEVENT;
    event_type = Game::InputEvent::MouseEvent;
    event_union = fb_mouse_event.Union();
    break;
  }

  default:
    return {};
  }

  auto fb_input_msg = Game::CreateInputMessage(builder, 0, 0.0F, type, event_type, event_union);

  auto fb_envelope =
    Game::CreateEnvelope(builder, Game::MsgType::INPUT, Game::MsgUnion::InputMessage, fb_input_msg.Union());

  builder.Finish(fb_envelope);
  auto buf = builder.Release();

  return { buf.begin(), buf.end() };
}

UUIDv4::UUID PacketHandler::FlatBufferSnapshotGetUUID(const uint8_t *data, std::size_t size)
{
  flatbuffers::Verifier verifier(data, size);
  if (!Game::VerifyEnvelopeBuffer(verifier)) { return UUIDv4::UUID{}; }

  const auto *envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::SNAPSHOT) { return UUIDv4::UUID{}; }

  const auto *snapshot = envelope->msg_as<Game::Snapshot>();
  if (snapshot == nullptr) { return UUIDv4::UUID{}; }

  return UUIDv4::UUID(snapshot->player_id()->str());
}

}// namespace chroma::shared::packet
