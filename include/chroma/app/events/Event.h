#pragma once

#include <cstdint>

namespace chroma::app::event {
class Event
{
public:
  enum Type : uint8_t {
    None = 0,
    KeyPressed = 1,
    KeyReleased = 2,
  };

  Event(const Event &) = default;
  Event(Event &&) = delete;
  Event &operator=(const Event &) = default;
  Event &operator=(Event &&) = delete;
  explicit Event(Event::Type type);
  virtual ~Event() = default;

  [[nodiscard]] Type GetType() const;
  [[nodiscard]] bool IsHandled() const;

  void SetType(Type type);
  void SetHandled(bool handled);

private:
  Type type_{ Type::None };
  bool handled_{ false };
};
}// namespace chroma::app::event
