#pragma once

#include "chroma/shared/render/RenderBridge.h"

#include <cstdint>
#include <functional>
#include <vector>

namespace chroma::client::render {
struct RenderCommand
{
  int layer;
  int sub_layer;
  float y;
  std::function<void()> draw_func;

  bool operator<(const RenderCommand &other) const
  {
    return std::tie(layer, y, sub_layer) < std::tie(other.layer, other.y, other.sub_layer);
  }
};

class RenderQueue
{
public:
  void Submit(int layer, int sub_layer, float y, const std::function<void()> &draw_func);
  void Submit(shared::render::RenderLayer layer, int sub_layer, float y, const std::function<void()> &draw_func);

  void Sort();
  void Execute() const;
  void Clear();

  [[nodiscard]] size_t Size() const { return commands_.size(); }

private:
  std::vector<RenderCommand> commands_;
};
}// namespace chroma::client::render
