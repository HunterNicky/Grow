#include "chroma/client/render/RenderQueue.h"

#include <algorithm>
#include <functional>

namespace chroma::client::render {
void RenderQueue::Submit(const int layer, const int sub_layer, const std::function<void()> &draw_func)
{
  commands_.push_back({ .layer = layer, .sub_layer = sub_layer, .draw_func = draw_func });
}

void RenderQueue::Submit(RenderLayer layer, const int sub_layer, const std::function<void()> &draw_func)
{
  Submit(static_cast<int>(layer), sub_layer, draw_func);
}

void RenderQueue::Sort()
{
  std::ranges::sort(commands_, [](const RenderCommand &a, const RenderCommand &b) { return a < b; });
}

void RenderQueue::Execute() const
{
  for (const auto &cmd : commands_) {
    if (cmd.draw_func) { cmd.draw_func(); }
  }
}

void RenderQueue::Clear() { commands_.clear(); }
}// namespace chroma::client::render