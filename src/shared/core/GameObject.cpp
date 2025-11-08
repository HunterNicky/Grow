#include <cstdint>
#include <memory>
#include <uuid_v4.h>

#include "chroma/shared/core/GameObject.h"
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/components/Transform.h"
#include "chroma/shared/utils/UUID.h"

namespace chroma::shared::core {

GameObject::GameObject()
  : active_(true), id_(utils::UUID::Generate()), layer_(0), Type_(GameObjectType::NONE), net_role_(NetRole::ROLE_None),
    transform_(std::make_shared<component::Transform>())
{}

GameObject::GameObject(const UUIDv4::UUID &id, bool active, uint32_t layer, GameObjectType tag)
  : active_(active), id_(id), layer_(layer), Type_(tag), net_role_(NetRole::ROLE_None),
    transform_(std::make_shared<component::Transform>())
{
  AttachComponent(transform_);
}

GameObject::~GameObject() { components_.clear(); }

void GameObject::SetActive(const bool active) { active_ = active; }

void GameObject::SetLayer(const uint32_t layer) { layer_ = layer; }

void GameObject::SetTag(const GameObjectType tag) { Type_ = tag; }

void GameObject::SetId(const UUIDv4::UUID &id_obj) { id_ = id_obj; }

void GameObject::SetNetRole(const NetRole role) { net_role_ = role; }

const UUIDv4::UUID &GameObject::GetId() const { return id_; }

bool GameObject::IsActive() const { return active_; }

uint32_t GameObject::GetLayer() const { return layer_; }

GameObjectType GameObject::GetTag() const { return Type_; }

NetRole GameObject::GetNetRole() const { return net_role_; }

bool GameObject::HasAuthority() const { return net_role_ == NetRole::ROLE_Authority; }

bool GameObject::IsAutonomousProxy() const { return net_role_ == NetRole::ROLE_AutonomousProxy; }

bool GameObject::IsSimulatedProxy() const { return net_role_ == NetRole::ROLE_SimulatedProxy; }

void GameObject::AttachComponent(const std::shared_ptr<component::Component> &component)
{
  if (component == nullptr) { return; }
  component->SetGameObject(shared_from_this());
  components_[component->GetType()] = component;
  component->Attach();
}

std::shared_ptr<component::Transform> GameObject::GetTransform() const { return transform_; }
}// namespace chroma::shared::core
