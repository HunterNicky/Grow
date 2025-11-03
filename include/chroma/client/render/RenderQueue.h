#pragma once
#include <cstdint>
#include <functional>
#include <vector>

namespace chroma::client::render {
enum class RenderLayer : uint16_t {
  Background = 0,
  Ground = 100,
  GroundDecoration = 200,
  Entities = 300,
  Effects = 400,
  UI = 500,
  Debug = 1000
};

struct RenderCommand
{
  int layer;
  int sub_layer;
  std::function<void()> draw_func;

  bool operator<(const RenderCommand &other) const
  {
    if (layer != other.layer) { return layer < other.layer; }
    return sub_layer < other.sub_layer;
  }
};

class RenderQueue
{
public:
  void Submit(int layer, int sub_layer, const std::function<void()> &draw_func);
  void Submit(RenderLayer layer, int sub_layer, const std::function<void()> &draw_func);

  void Sort();
  void Execute() const;
  void Clear();

  [[nodiscard]] size_t Size() const { return commands_.size(); }

private:
  std::vector<RenderCommand> commands_;
};
}// namespace chroma::client::render
