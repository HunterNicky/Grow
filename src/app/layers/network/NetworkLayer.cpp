#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::network {
NetworkLayer::NetworkLayer() : Layer("NetworkLayer") {}

NetworkLayer::~NetworkLayer() = default;

void NetworkLayer::OnUpdate(float delta_time)
{
  if (!IsActive()) { return; }

  state_machine_->OnUpdate(delta_time);
}
}// namespace chroma::app::layer::network