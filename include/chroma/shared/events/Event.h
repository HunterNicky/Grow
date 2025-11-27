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
    ProjectileEvent = 5,
    ShaderEvent = 6,
    WaveEvent = 7,
    ButtonClickEvent = 8,
    StateEvent = 9,
    LayerEvent = 10,
    PanelEvent = 11,
    AudioVolumeEvent = 12,
    SaveSettingsEvent = 13,
    PlayerDataEvent = 14,
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
