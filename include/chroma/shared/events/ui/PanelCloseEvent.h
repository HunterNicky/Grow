#pragma once

#include "chroma/shared/events/Event.h"

#include <string>
#include <memory>

namespace chroma::shared::event::ui {
class PanelCloseEvent : public Event
{
public:
  explicit PanelCloseEvent(std::string panel_id);

  [[nodiscard]] std::string &GetId();
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  std::string panel_id_;
};
}// namespace chroma::shared::event::ui