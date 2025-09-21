#include <memory>
#include <string>
#include <utility>
#include <algorithm>

#include "chroma/app/events/Event.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/LayerStack.h"

namespace chroma::app::layer {
void LayerStack::PushLayer(std::unique_ptr<Layer> layer) { layers_.emplace_back(std::move(layer)); }

void LayerStack::PopLayer()
{
  if (!layers_.empty()) { layers_.pop_back(); }
}

void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay) { overlays_.emplace_back(std::move(overlay)); }

void LayerStack::PopOverlay()
{
  if (!overlays_.empty()) { overlays_.pop_back(); }
}

void LayerStack::UpdateLayers(const float deltaTime) const
{
  for (const auto &layer : layers_) {
    if (layer->IsActive()) { layer->OnUpdate(deltaTime); }
  }
  for (const auto &overlay : overlays_) {
    if (overlay->IsActive()) { overlay->OnUpdate(deltaTime); }
  }
}

void LayerStack::RenderLayers() const
{
  for (const auto &layer : layers_) {
    if (layer->IsActive()) { layer->OnRender(); }
  }
  for (const auto &overlay : overlays_) {
    if (overlay->IsActive()) { overlay->OnRender(); }
  }
}

void LayerStack::HandleEvent(event::Event &event) const
{
  for (const auto &overlay : overlays_) {
    if (overlay->IsActive()) { overlay->OnEvent(event); }
    if (event.IsHandled()) { return; }
  }
  for (const auto &layer : layers_) {
    if (layer->IsActive()) { layer->OnEvent(event); }
    if (event.IsHandled()) { return; }
  }
}

Layer *LayerStack::GetLayer(const std::string &name) const
{
  auto iterator = std::ranges::find_if(layers_,
                         [&](const std::unique_ptr<Layer> &layer) { return layer->GetName() == name; });
  if (iterator != layers_.end()) { return iterator->get(); }

  iterator = std::ranges::find_if(overlays_,
                    [&](const std::unique_ptr<Layer> &overlay) { return overlay->GetName() == name; });
  if (iterator != overlays_.end()) { return iterator->get(); }

  return nullptr;
}
}// namespace chroma::app::layer
