#pragma once

#include <string>

#include "chroma/shared/events/Event.h"

namespace chroma::shared::event::ui {
class ButtonClickEvent : public Event
{
public:
  explicit ButtonClickEvent(Event::Type type, std::string id);
  ButtonClickEvent();
  
  [[nodiscard]] std::string& GetId();
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  std::string id_;
};
}