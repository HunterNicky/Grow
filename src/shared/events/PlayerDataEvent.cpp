#include "chroma/shared/events/PlayerDataEvent.h"
#include "chroma/app/database/DatabaseTypes.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event {

PlayerDataEvent::PlayerDataEvent(const PlayerDataAction action, const app::database::PlayerData &data)
  : Event(Type::PlayerDataEvent), action_(action), data_(data)
{}

PlayerDataAction PlayerDataEvent::GetAction() const { return action_; }

const app::database::PlayerData &PlayerDataEvent::GetData() const { return data_; }

std::shared_ptr<Event> PlayerDataEvent::Clone() const { return std::make_shared<PlayerDataEvent>(action_, data_); }

Event::Type PlayerDataEvent::GetStaticType() { return Type::PlayerDataEvent; }

}// namespace chroma::shared::event