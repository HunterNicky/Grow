#include "chroma/shared/events/SaveSettingsEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event {

SaveSettingsEvent::SaveSettingsEvent() : Event(Type::SaveSettingsEvent) {}

std::shared_ptr<Event> SaveSettingsEvent::Clone() const { return std::make_shared<SaveSettingsEvent>(*this); }

Event::Type SaveSettingsEvent::GetStaticType() { return Event::SaveSettingsEvent; }
}// namespace chroma::shared::event