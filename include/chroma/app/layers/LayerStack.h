#pragma once

#include <memory>
#include <string>
#include <vector>

#include "chroma/app/layers/Layer.h"

namespace chroma::app::layer {
class LayerStack
{
public:
  LayerStack() = default;

  void PushLayer(std::unique_ptr<Layer> layer);
  void PopLayer();

  void PushOverlay(std::unique_ptr<Layer> overlay);
  void PopOverlay();

  void UpdateLayers(float delta_time) const;
  void UpdateFixedLayers(float fixed_delta_time) const;
  void RenderLayers() const;
  void HandleEvent(shared::event::Event &event) const;

  [[nodiscard]] Layer *GetLayer(const std::string &name) const;

private:
  std::vector<std::unique_ptr<Layer>> layers_;
  std::vector<std::unique_ptr<Layer>> overlays_;
};
}// namespace chroma::app::layer
