#pragma once

#include "chroma/shared/events/Event.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"

#include <memory>

namespace chroma::shared::event::ui {

enum class Action { Close = 0, Open = 1 };

class PanelEvent : public Event
{
public:
  explicit PanelEvent(const Action action, const client::ui::panel::PanelID panel_id);

  [[nodiscard]] client::ui::panel::PanelID &GetPanelId();
  [[nodiscard]] Action GetAction() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  Action action_;
  client::ui::panel::PanelID panel_id_;
};
}// namespace chroma::shared::event::ui