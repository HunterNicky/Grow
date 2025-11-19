#include "chroma/shared/events/ui/PanelCloseEvent.h"
#include "chroma/shared/events/Event.h"

#include <string>
#include <utility>
#include <memory>

namespace chroma::shared::event::ui {
PanelCloseEvent::PanelCloseEvent(std::string panel_id)
  : Event(Event::Type::PanelCloseEvent), panel_id_(std::move(panel_id))
{}

std::string& PanelCloseEvent::GetId() { return panel_id_; }

std::shared_ptr<Event> PanelCloseEvent::Clone() const { return std::make_shared<PanelCloseEvent>(*this); }

Event::Type PanelCloseEvent::GetStaticType() { return Event::PanelCloseEvent; }
}// namespace chroma::shared::event::ui