#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include "GameObject_generated.h"
#include "chroma/shared/core/components/Speed.h"
#include "chroma/shared/core/player/Player.h"

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

    std::vector<flatbuffers::Offset<Game::EntityState>> fbEntities;

    for (const auto& object : objects) {
        auto pos = object->GetComponent<component::Transform>();
        auto vel = object->GetComponent<component::Speed>();

        std::vector<flatbuffers::Offset<Game::Component>> components;

        if (pos) {
            auto vec2 = Game::CreateVec2(builder, pos->GetPosition().x, pos->GetPosition().y);
            auto fbPos = Game::CreatePosition(builder, vec2);
            components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Position, fbPos.Union()));
        }

        if (vel) {
            auto vec2 = Game::CreateVec2(builder, vel->GetSpeed().x, vel->GetSpeed().y);
            auto fbVel = Game::CreateVelocity(builder, vec2);
            components.push_back(Game::CreateComponent(builder, Game::ComponentUnion::Velocity, fbVel.Union()));
        }

        auto fbId = builder.CreateString(object->GetId().str());
        auto fbType = static_cast<Game::GameObjectType>(object->GetTag());
        auto fbComponents = builder.CreateVector(components);
        auto fbEntity = Game::CreateEntityState(builder, fbId, fbType, fbComponents);
        fbEntities.push_back(fbEntity);
    }

    auto fbEntitiesVec = builder.CreateVector(fbEntities);
    auto snapshot = Game::CreateSnapshot(builder, time_lapse, last_processed_input, fbEntitiesVec);
    auto envelope = Game::CreateEnvelope(builder, Game::MsgType::SNAPSHOT, Game::MsgUnion::Snapshot, snapshot.Union());
    builder.Finish(envelope);

    auto buf = builder.Release();

    return std::vector<uint8_t>(buf.begin(), buf.end());
}

void PacketHandler::FlatBufferToGameObject(const uint8_t* data, std::vector<std::shared_ptr<chroma::shared::core::GameObject>>& gameObjects)
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

  for (const auto& entityState : *snapshot->entities()) {
    auto it = std::ranges::find_if(gameObjects, [&](const std::shared_ptr<GameObject>& obj) {
        return static_cast<Game::GameObjectType>(obj->GetTag()) == entityState->type();
    });

    if (it != std::end(gameObjects)) {
        UpdateGameObjectWithEntityState(entityState, *it);
    } else {

        std::shared_ptr<chroma::shared::core::GameObject> gameObject = nullptr;
        switch (entityState->type()) {
          case Game::GameObjectType::PLAYER: {
              gameObject = std::make_shared<chroma::shared::core::player::Player>();
              gameObjects.push_back(gameObject);
              break;
          }
        }   
        UpdateGameObjectWithEntityState(entityState, gameObject);
    }
  }
}

// NOLINTNEXTLINE
void PacketHandler::UpdateGameObjectWithEntityState(const Game::EntityState* entityState, std::shared_ptr<chroma::shared::core::GameObject>& gameObject)
{
  switch (entityState->type()) {
    case Game::GameObjectType::PLAYER: {
        auto player = std::dynamic_pointer_cast<chroma::shared::core::player::Player>(gameObject);
        if (player) {
            player->SetId(UUIDv4::UUID(entityState->id()->str()));
            
            for (const auto& component : *entityState->components()) {
                switch (component->type_type()) {
                    case Game::ComponentUnion::Position: {
                        const auto* fbPos = component->type_as_Position();
                        if (fbPos != nullptr) {
                            const auto* posVec = fbPos->pos();
                            auto transform = player->GetComponent<component::Transform>();
                            if (transform) {
                                transform->SetPosition(Vector2{posVec->x(), posVec->y()});
                            }
                        }
                        break;
                    }
                    case Game::ComponentUnion::Velocity: {
                        const auto* fbVel = component->type_as_Velocity();
                        if (fbVel != nullptr) {
                            const auto* velVec = fbVel->vel();
                            auto speed = player->GetComponent<component::Speed>();
                            if (speed) {
                                speed->SetSpeed(Vector2{velVec->x(), velVec->y()});
                            }
                        }
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


} // namespace chroma::server::packet
