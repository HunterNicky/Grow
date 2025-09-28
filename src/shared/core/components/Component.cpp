#include <memory>

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Component.h"
#include "uuid_v4.h"

namespace chroma::shared::core::component {
Component::Component() : type_(ComponentType::NONE) {}

ComponentType Component::GetType() const { return type_; }
bool Component::IsType(ComponentType type) const { return type_ == type; }

UUIDv4::UUID Component::GetId() const { return id_; }

void Component::SetGameObject(const std::shared_ptr<GameObject> &game_object) { game_object_ = game_object; }
std::shared_ptr<GameObject> Component::GetGameObject() const { return game_object_.lock(); }

bool Component::operator==(const Component &other) const { return id_ == other.id_; }

void Component::Attach() {}
void Component::Detach() {}

void Component::Update([[maybe_unused]] float delta_time) {}
void Component::FixedUpdate([[maybe_unused]] float fixed_delta_time) {}
void Component::Render() {}
}// namespace chroma::shared::core::component
