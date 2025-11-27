#include "chroma/shared/events/state/StateEvent.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::state {
StateEvent::StateEvent(const Action action)
  : Event(Type::StateEvent), action_(action), state_id_(app::states::StateID::None)
{}
StateEvent::StateEvent(const Action action, const app::states::StateID state_id)
  : Event(Type::StateEvent), action_(action), state_id_(state_id)
{}

app::states::StateID &StateEvent::GetStateId() { return state_id_; }
Action StateEvent::GetAction() const { return action_; }
std::shared_ptr<Event> StateEvent::Clone() const { return std::make_shared<StateEvent>(*this); }

Event::Type StateEvent::GetStaticType() { return Event::StateEvent; }
}// namespace chroma::shared::event::state