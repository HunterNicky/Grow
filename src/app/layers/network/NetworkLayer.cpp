#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/states/StateIdentifiers.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/state/StateEvent.h"

#include <memory>

namespace chroma::app::layer::network {
NetworkLayer::NetworkLayer() : Layer("NetworkLayer")
{
  state_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::state::StateEvent>(
    [this](shared::event::state::StateEvent &event) { this->OnStateEvent(event); });
}

NetworkLayer::~NetworkLayer() = default;

void NetworkLayer::OnUpdate(const float delta_time)
{
  if (!IsActive()) { return; }

  state_machine_->OnUpdate(delta_time);
}

void NetworkLayer::RegisterStates() {}

void NetworkLayer::OnStateEvent(shared::event::state::StateEvent &state_event)
{
  if (state_event.GetStateId() != states::StateID::PauseState) { return; }

  const auto network_state = std::dynamic_pointer_cast<states::NetworkState>(state_machine_->GetCurrentState());

  network_state->SetDispatchEvent(state_event.GetAction() == shared::event::state::Action::Pop);
}


}// namespace chroma::app::layer::network