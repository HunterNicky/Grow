#pragma once

#include "chroma/shared/events/Event.h"

#include <cstdint>
#include <memory>

namespace chroma::shared::event {

class KeyEvent : public Event
{
public:
  explicit KeyEvent(uint8_t key);
  KeyEvent();
  [[nodiscard]] uint8_t GetKey() const;
  [[nodiscard]] bool IsPressed() const;
  [[nodiscard]] bool IsReleased() const;

  [[nodiscard]] std::shared_ptr<Event> Clone() const override;

  [[nodiscard]] static Event::Type GetStaticType();

  void SetPressed(bool pressed);

private:
  uint8_t key_;
  bool pressed_ = false;
};

}// namespace chroma::shared::event
