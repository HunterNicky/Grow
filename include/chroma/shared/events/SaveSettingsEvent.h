#pragma once

#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {

class SaveSettingsEvent : public Event
{
public:
  SaveSettingsEvent();
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();
};
}// namespace chroma::shared::event