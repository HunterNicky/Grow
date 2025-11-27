#include "chroma/shared/events/PlayerDataEvent.h"
#include "chroma/shared/events/Event.h"

namespace chroma::shared::event {

PlayerDataEvent::PlayerDataEvent(PlayerDataAction action, const app::database::PlayerData &data)
  : Event(Type::PlayerDataEvent), action_(action), data_(data)
{}

PlayerDataAction PlayerDataEvent::GetAction() const { return action_; }

const app::database::PlayerData &PlayerDataEvent::GetData() const { return data_; }

std::shared_ptr<Event> PlayerDataEvent::Clone() const { return std::make_shared<PlayerDataEvent>(action_, data_); }

Event::Type PlayerDataEvent::GetStaticType() { return Event::Type::PlayerDataEvent; }

}// namespace chroma::shared::event