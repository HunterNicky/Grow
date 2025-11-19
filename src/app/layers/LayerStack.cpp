#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/shared/events/layer/PushLayerEvent.h"
#include "chroma/shared/events/layer/PopLayerEvent.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/layers/LayerStack.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/shared/events/Event.h"

namespace chroma::app::layer {
LayerStack::LayerStack() : command_queue_(std::make_unique<command::CommandQueue>()) 
{
}

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

void LayerStack::UpdateLayers(const float delta_time) const
{
  command_queue_->Process();
  for (const auto &layer : layers_) {
    if (layer->IsActive()) { layer->OnUpdate(delta_time); }
  }
  for (const auto &overlay : overlays_) {
    if (overlay->IsActive()) { overlay->OnUpdate(delta_time); }
  }
}

void LayerStack::UpdateFixedLayers(const float fixed_delta_time) const
{
  command_queue_->Process();
  for (const auto &layer : layers_) {
    if (layer->IsActive()) { layer->OnFixedUpdate(fixed_delta_time); }
  }
  for (const auto &overlay : overlays_) {
    if (overlay->IsActive()) { overlay->OnFixedUpdate(fixed_delta_time); }
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

void LayerStack::HandleEvent(shared::event::Event &event) const
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
  auto iterator =
    std::ranges::find_if(layers_, [&](const std::unique_ptr<Layer> &layer) { return layer->GetName() == name; });
  if (iterator != layers_.end()) { return iterator->get(); }

  iterator =
    std::ranges::find_if(overlays_, [&](const std::unique_ptr<Layer> &overlay) { return overlay->GetName() == name; });
  if (iterator != overlays_.end()) { return iterator->get(); }

  return nullptr;
}

void LayerStack::SetEventPushLayer()
{
    push_layer_subscription_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::layer::PushLayerEvent>(
    [this](shared::event::layer::PushLayerEvent &event) { this->PushLayerEvent(event); });
}

void LayerStack::SetEventPopLayer()
{
    pop_layer_subscription_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::layer::PopLayerEvent>(
    [this](shared::event::layer::PopLayerEvent &event) { this->PopLayerEvent(event); });
}

void LayerStack::PushLayerEvent(const shared::event::layer::PushLayerEvent &event)
{
   if(event.GetLayerId() == "SinglePlayerGameLayer")
   {
      auto game_state = std::make_shared<states::GameState>();
      auto game_layer = std::make_unique<layer::game::GameLayer>();
      game_layer->PushState(game_state);
      if (!layers_.empty()) {
          layers_.pop_back();
      }
      PushLayer(std::move(game_layer));
   } else if(event.GetLayerId() == "MultiPlayerGameLayer")
   {
      auto game_layer = std::make_unique<layer::game::GameLayer>();
      auto network_layer = std::make_unique<layer::network::NetworkLayer>();

      auto mediator = std::make_shared<states::GameNetworkMediator>();
      const auto game_state = std::make_shared<states::GameState>(mediator);
      game_state->SetEventDispatcher();
      const auto network_state = std::make_shared<states::NetworkState>(mediator);
      network_state->SetEventDispatcher();

      mediator->SetGameState(game_state);
      mediator->SetNetworkState(network_state);

      network_layer->PushState(network_state);
      game_layer->PushState(game_state);

      PushLayer(std::move(network_layer));
      PushLayer(std::move(game_layer));

   }
}

void LayerStack::PopLayerEvent(const shared::event::layer::PopLayerEvent &event)
{
  (event);
   PopLayer();
}

}// namespace chroma::app::layer
