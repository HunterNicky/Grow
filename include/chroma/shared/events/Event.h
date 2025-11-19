#pragma once

#include <cstdint>
#include <memory>

namespace chroma::shared::event {
class Event
{
public:
  enum Type : uint8_t {
    None = 0,
    KeyEvent = 1,
    MouseClickEvent = 2,
    MouseEvent = 3,
    SoundEvent = 4,
    ButtonClickEvent = 5,
    PanelOpenEvent = 6,
    PanelCloseEvent = 7,
    PushStateEvent = 8,
    PopStateEvent = 9,
    PopLayerEvent = 10,
    PushLayerEvent = 11,
  };

  Event() = default;
  Event(const Event &) = default;
  Event(Event &&) = delete;
  Event &operator=(const Event &) = default;
  Event &operator=(Event &&) = delete;
  explicit Event(Type type);
  virtual ~Event() = default;

  [[nodiscard]] virtual std::shared_ptr<Event> Clone() const = 0;

  [[nodiscard]] Type GetType() const;
  [[nodiscard]] bool IsHandled() const;

  void SetType(Type type);
  void SetHandled(bool handled);

private:
  Type type_{ None };
  bool handled_{ false };
};
}// namespace chroma::shared::event
