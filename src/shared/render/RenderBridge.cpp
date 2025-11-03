#include "chroma/shared/render/RenderBridge.h"

#include <memory>
#include <utility>

namespace chroma::shared::render {

static std::shared_ptr<IRenderBridge> &BridgeStorage()
{
  static std::shared_ptr<IRenderBridge> s_bridge;
  return s_bridge;
}

void SetRenderBridge(std::shared_ptr<IRenderBridge> bridge) { BridgeStorage() = std::move(bridge); }

std::shared_ptr<IRenderBridge> GetRenderBridge() { return BridgeStorage(); }

} // namespace chroma::shared::render
