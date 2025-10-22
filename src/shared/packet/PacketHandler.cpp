#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include "GameObject_generated.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/packet/ImputMessage.h"

#include <algorithm>
#include <cstdint>
#include <flatbuffers/flatbuffers.h>
#include <raylib.h>
#include <vector>
#include <memory>

using namespace chroma::shared::core;

namespace chroma::server::packet {

std::vector<uint8_t> PacketHandler::GameObjectsToFlatBuffer(const std::vector<std::shared_ptr<GameObject>>& objects,  uint64_t time_lapse, uint32_t last_processed_input)
{
    flatbuffers::FlatBufferBuilder builder(1024);

    std::vector<flatbuffers::Offset<Game::EntityState>> fb_entities;

    for (const auto& object : objects) {
        auto pos = object->GetComponent<component::Transform>();
        auto vel = object->GetComponent<component::Speed>();

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

    auto fb_entities_vec = builder.CreateVector(fb_entities);
    auto snapshot = Game::CreateSnapshot(builder, time_lapse, last_processed_input, fb_entities_vec);
    auto envelope = Game::CreateEnvelope(builder, Game::MsgType::SNAPSHOT, Game::MsgUnion::Snapshot, snapshot.Union());
    builder.Finish(envelope);

    auto buf = builder.Release();

    return std::vector<uint8_t>(buf.begin(), buf.end());
}

void PacketHandler::FlatBufferToGameObject(const uint8_t* data, std::vector<std::shared_ptr<chroma::shared::core::GameObject>>& game_objects)
{

  flatbuffers::Verifier verifier(data, 1024);
  if (!Game::VerifyEnvelopeBuffer(verifier)) {
      return;
  }

  const auto* envelope = Game::GetEnvelope(data);
  if (envelope->type() != Game::MsgType::SNAPSHOT) {
      return;
  }

  const auto* snapshot = envelope->msg_as<Game::Snapshot>();
  if (snapshot == nullptr) {
      return;
  }

  for (const auto& entity_state : *snapshot->entities()) {
    auto it = std::ranges::find_if(game_objects, [&](const std::shared_ptr<GameObject>& obj) {
        return static_cast<Game::GameObjectType>(obj->GetTag()) == entity_state->type();
    });

    if (it != std::end(game_objects)) {
        UpdateGameObjectWithEntityState(entity_state, *it);
    } else {

        std::shared_ptr<chroma::shared::core::GameObject> game_object = nullptr;
        switch (entity_state->type()) {
          case Game::GameObjectType::PLAYER: {
              game_object = std::make_shared<chroma::shared::core::player::Player>();
              game_objects.push_back(game_object);
              break;
          }
        }   
        UpdateGameObjectWithEntityState(entity_state, game_object);
    }
  }
}

void PacketHandler::UpdateGameObjectWithEntityState(const Game::EntityState* entity_state, std::shared_ptr<chroma::shared::core::GameObject>& game_object)
{
  switch (entity_state->type()) {
    case Game::GameObjectType::PLAYER: {
        auto player = std::dynamic_pointer_cast<chroma::shared::core::player::Player>(game_object);
        if (player) {
            player->SetId(UUIDv4::UUID(entity_state->id()->str()));
            
            for (const auto& component : *entity_state->components()) {
                switch (component->type_type()) {
                    case Game::ComponentUnion::Position: {
                        ComponentToTransform(component, game_object);
                        break;
                    }
                    case Game::ComponentUnion::Velocity: {
                        ComponentToSpeed(component, game_object);
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

void PacketHandler::ComponentToSpeed(const Game::Component* component, std::shared_ptr<chroma::shared::core::GameObject>& game_object)
{
    const auto* fb_vel = component->type_as_Velocity();
    if (fb_vel != nullptr) {
        const auto* vel_vec = fb_vel->vel();
        auto speed = game_object->GetComponent<component::Speed>();
        if (speed) {
            speed->SetSpeed(Vector2{vel_vec->x(), vel_vec->y()});
        }
    }
}

void PacketHandler::ComponentToTransform(const Game::Component* component, std::shared_ptr<chroma::shared::core::GameObject>& game_object)
{
    const auto* fb_pos = component->type_as_Position();
    if (fb_pos != nullptr) {
        const auto* pos_vec = fb_pos->pos();
        auto transform = game_object->GetComponent<component::Transform>();
        if (transform) {
            transform->SetPosition(Vector2{pos_vec->x(), pos_vec->y()});
        }
    }
}

std::shared_ptr<shared::packet::InputMessage> PacketHandler::FlatBufferToInputMessage(const uint8_t* data)
{
    flatbuffers::Verifier verifier(data, 1024);
    if (!Game::VerifyEnvelopeBuffer(verifier)) {
        return nullptr;
    }

    const auto* envelope = Game::GetEnvelope(data);
    if (envelope->type() != Game::MsgType::INPUT) {
        return nullptr;
    }

    const auto* input_msg = envelope->msg_as<Game::InputMessage>();
    
    if (input_msg == nullptr) {
        return nullptr;
    }

    std::shared_ptr<shared::packet::InputMessage> input_message = std::make_unique<shared::packet::InputMessage>();
    
    input_message->SetSeq(input_msg->seq());
    input_message->SetDeltaTime(input_msg->dt());
    input_message->SetPlayerId(input_msg->player_id()->str());

    if (input_msg->event_type() == Game::InputEvent::KeyEvent) {
        shared::event::KeyEvent key_event;
        const auto* fb_key_event = input_msg->event_as_KeyEvent();
        if (fb_key_event != nullptr) {
            key_event = shared::event::KeyEvent(fb_key_event->key());
            key_event.SetPressed(fb_key_event->is_pressed());
            input_message->SetEventType(shared::event::Event::Type::KeyEvent);
            input_message->SetKeyEvent(key_event);
        }
    } else if (input_msg->event_type() == Game::InputEvent::MouseEvent) {
        shared::event::MouseEvent mouse_event;
        const auto* fb_mouse_event = input_msg->event_as_MouseEvent();
        if (fb_mouse_event != nullptr) {
            Vector2 position{fb_mouse_event->mouse_position()->x(), fb_mouse_event->mouse_position()->y()};
            mouse_event = shared::event::MouseEvent(position, fb_mouse_event->left(), fb_mouse_event->right());
            input_message->SetEventType(shared::event::Event::Type::MouseEvent);
            input_message->SetMouseEvent(mouse_event);
        }
    }

    return input_message;
}
} // namespace chroma::server::packet
