#include "chroma/shared/events/LevelSelectEvent.h"
#include "chroma/shared/events/Event.h"

#include <memory>
#include <string>
#include <utility>

namespace chroma::shared::event {

LevelSelectEvent::LevelSelectEvent(std::string level_id)
  : Event(Type::LevelSelectEvent), level_id_(std::move(level_id))
{}

const std::string &LevelSelectEvent::GetLevelId() const { return level_id_; }

std::shared_ptr<Event> LevelSelectEvent::Clone() const { return std::make_shared<LevelSelectEvent>(*this); }

Event::Type LevelSelectEvent::GetStaticType() { return Event::LevelSelectEvent; }

}// namespace chroma::shared::event
