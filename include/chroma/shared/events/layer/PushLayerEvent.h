#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>

namespace chroma::shared::event::layer {
class PushLayerEvent : public Event
{
public:
  explicit PushLayerEvent(std::string layer_id);

  [[nodiscard]] const std::string &GetLayerId() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  std::string layer_id_;
};
}// namespace chroma::shared::event::state