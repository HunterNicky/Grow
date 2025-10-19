#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::network {
NetworkLayer::NetworkLayer() : Layer("NetworkLayer") {}

void NetworkLayer::OnUpdate(float delta_time) 
{
    if(!IsActive()) {return;}

    state_machine_->OnUpdate(delta_time);
}

void NetworkLayer::OnEvent(event::Event& event)
{
    if (!IsActive()) { return; }

    auto current_state = GetCurrentState();

    if (current_state) { current_state->OnEvent(event); }
}
} // namespace chroma::app::layer::network