#pragma once

#include <cstdint>
#include <memory>
#include <uuid_v4.h>

namespace chroma::shared::core {
class GameObject;
}

namespace chroma::shared::core::component {
enum class ComponentType : uint8_t {
  NONE = 0,
  TRANSFORM = 1,
  SPEED = 2,
  MOVEMENT = 3,
  SPRITE_STATIC = 4,
  SPRITE_ANIMATION = 5,
  CAMERA = 6,
  AUDIO_LISTENER = 7,
};

class Component
{
public:
  Component();
  Component(const Component &) = default;
  Component(Component &&) = delete;
  Component &operator=(const Component &) = default;
  Component &operator=(Component &&) = delete;
  virtual ~Component() = default;

  [[nodiscard]] ComponentType GetType() const;
  [[nodiscard]] bool IsType(ComponentType type) const;
  [[nodiscard]] UUIDv4::UUID GetId() const;

  void SetGameObject(const std::shared_ptr<GameObject> &game_object);
  [[nodiscard]] std::shared_ptr<GameObject> GetGameObject() const;

  bool operator==(const Component &other) const;

  virtual void Attach();
  virtual void Detach();

  virtual void Update([[maybe_unused]] float delta_time);
  virtual void FixedUpdate([[maybe_unused]] float fixed_delta_time);
  virtual void Render();

protected:
  ComponentType type_;

  UUIDv4::UUID id_;
  std::weak_ptr<GameObject> game_object_;
};
}// namespace chroma::shared::core::component
