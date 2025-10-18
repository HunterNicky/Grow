#include "chroma/shared/packet/PacketHandler.h"
#include "chroma/shared/core/GameObject.h"
#include "GameObject_generated.h"
#include "chroma/shared/core/components/Speed.h"

#include <flatbuffers/flatbuffers.h>
#include <raylib.h>
#include <vector>
#include <memory>

using namespace chroma::shared::core;

namespace chroma::server::packet {

std::vector<uint8_t> PacketHandler::GameObjectToFlatBuffer(const GameObject& object)
{
    flatbuffers::FlatBufferBuilder builder(1024);

    auto pos = object.GetComponent<component::Transform>();
    auto vel = object.GetComponent<component::Speed>();

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

    auto fbId = builder.CreateString(object.GetId().str());
    auto fbComponents = builder.CreateVector(components);
    auto fbEntity = Game::CreateEntityState(builder, fbId, fbComponents);
    auto fbEntities = builder.CreateVector(&fbEntity, 1);

    auto snapshot = Game::CreateSnapshot(builder, 0, 0, fbEntities);
    auto envelope = Game::CreateEnvelope(builder, Game::MsgType::SNAPSHOT, Game::MsgUnion::Snapshot, snapshot.Union());
    builder.Finish(envelope);

    auto buf = builder.Release();

    return std::vector<uint8_t>(buf.begin(), buf.end());
}

std::unique_ptr<GameObject> PacketHandler::FlatBufferToGameObject(const uint8_t* data, size_t size)
{
   return nullptr;
}

} // namespace chroma::server::packet
