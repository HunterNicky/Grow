#include "chroma/shared/events/ui/PanelOpenEvent.h"
#include "chroma/shared/events/Event.h"

#include <string>
#include <utility>
#include <memory>

namespace chroma::shared::event::ui {
PanelOpenEvent::PanelOpenEvent(std::string panel_id)
  : Event(Event::Type::PanelOpenEvent), panel_id_(std::move(panel_id))
{}

std::string& PanelOpenEvent::GetId() { return panel_id_; }

std::shared_ptr<Event> PanelOpenEvent::Clone() const { return std::make_shared<PanelOpenEvent>(*this); }

Event::Type PanelOpenEvent::GetStaticType() { return Event::PanelOpenEvent; }
}// namespace chroma::shared::event::ui