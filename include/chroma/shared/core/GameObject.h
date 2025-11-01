#pragma once

#include "chroma/shared/core/components/Transform.h"

#include <cstdint>
#include <map>
#include <memory>
#include <uuid_v4.h>

namespace chroma::shared::collision {
struct CollisionEvent;
}// namespace chroma::shared::collision

namespace chroma::shared::core {

namespace component {
  enum class ComponentType : uint8_t;
  class Component;
  class Transform;
}// namespace component

enum class GameObjectType : uint8_t {
  NONE = 0,
  PLAYER = 1,
  ENEMY = 2,
  PROJECTILE = 3,
  OBSTACLE = 4,
};

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
  GameObject();
  GameObject(const GameObject &) = default;
  GameObject(GameObject &&) = delete;
  GameObject &operator=(const GameObject &) = default;
  GameObject &operator=(GameObject &&) = delete;
  GameObject(const UUIDv4::UUID &id, bool active, uint32_t layer, GameObjectType tag);
  virtual ~GameObject();

  void SetActive(bool active);
  void SetLayer(uint32_t layer);
  void SetTag(GameObjectType tag);
  void SetId(const UUIDv4::UUID &id_obj);

  [[nodiscard]] const UUIDv4::UUID &GetId() const;
  [[nodiscard]] bool IsActive() const;
  [[nodiscard]] uint32_t GetLayer() const;
  [[nodiscard]] GameObjectType GetTag() const;

  void AttachComponent(const std::shared_ptr<component::Component> &component);

  template<typename T> std::shared_ptr<T> GetComponent() const
  {
    static_assert(std::is_base_of_v<component::Component, T>, "T must be a Component");
    auto it = components_.find(T().GetType());
    if (it != components_.end()) { return std::dynamic_pointer_cast<T>(it->second); }
    return nullptr;
  }

  std::shared_ptr<core::component::Transform> GetTransform() const;

  virtual void OnUpdate(float delta_time) = 0;
  virtual void OnFixedUpdate(float fixed_delta_time) = 0;
  virtual void OnCollision(const collision::CollisionEvent &collision) = 0;
  virtual void OnRender() = 0;

protected:
  bool active_;

  UUIDv4::UUID id_;
  uint32_t layer_;
  GameObjectType Type_;

  std::map<component::ComponentType, std::shared_ptr<component::Component>> components_;
  std::shared_ptr<core::component::Transform> transform_;
};
}// namespace chroma::shared::core
