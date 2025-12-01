#include <string>
#include <utility>
#include <memory>

#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/Event.h"

namespace chroma::shared::event::ui {
ButtonClickEvent::ButtonClickEvent(const Type type, std::string id) : Event(type), id_(std::move(id)) {}

ButtonClickEvent::ButtonClickEvent() : Event(Event::ButtonClickEvent), id_("Blank") {}

std::string &ButtonClickEvent::GetId() { return id_; }

std::shared_ptr<Event> ButtonClickEvent::Clone() const { return std::make_shared<ButtonClickEvent>(*this); }

Event::Type ButtonClickEvent::GetStaticType() { return Event::ButtonClickEvent; }
}// namespace chroma::shared::event