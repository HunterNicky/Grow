#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::layer {
class PopLayerEvent : public Event
{
public:
  PopLayerEvent();

  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();
};
}// namespace chroma::shared::event::state