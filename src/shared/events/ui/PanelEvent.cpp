#include "chroma/shared/events/ui/PanelEvent.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::ui {
PanelEvent::PanelEvent(const Action action, const client::ui::panel::PanelID panel_id)
  : Event(Type::PanelEvent), action_(action), panel_id_(panel_id)
{}

client::ui::panel::PanelID &PanelEvent::GetPanelId() { return panel_id_; }
Action PanelEvent::GetAction() const { return action_; }
std::shared_ptr<Event> PanelEvent::Clone() const { return std::make_shared<PanelEvent>(*this); }

Event::Type PanelEvent::GetStaticType() { return Event::PanelEvent; }
}// namespace chroma::shared::event::ui