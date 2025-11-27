#pragma once

#include "chroma/shared/events/Event.h"
#include "chroma/app/states/StateIdentifiers.h"

#include <memory>

namespace chroma::shared::event::state {

enum class Action { Pop = 0, Push = 1 };

class StateEvent : public Event
{
public:
  explicit StateEvent(const Action action);
  StateEvent(const Action action, const app::states::StateID state_id);

  [[nodiscard]] app::states::StateID &GetStateId();
  [[nodiscard]] Action GetAction() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  Action action_;
  app::states::StateID state_id_;
};
}// namespace chroma::shared::event::state