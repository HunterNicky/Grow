#include "chroma/app/layers/states/network/NetworkLayer.h"

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer::states {

NetWorkLayer::NetWorkLayer() : Layer("NetWorkLayer") {}

void NetWorkLayer::OnUpdate(const float delta_time) {
    (void)delta_time;
}

void NetWorkLayer::OnRender() {}

} // namespace chroma::app::layer::states