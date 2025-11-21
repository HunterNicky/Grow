#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "chroma/app/commands/CommandQueue.h"
#include "chroma/app/commands/FunctionalCommand.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/LayerIdentifiers.h"
#include "chroma/app/layers/LayerStack.h"
#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/layer/LayerEvent.h"

namespace chroma::app::layer {
LayerStack::LayerStack() : command_queue_(std::make_unique<command::CommandQueue>()) {}

void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
{
  layer->OnAttach();
  layers_.emplace_back(std::move(layer));
}

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

void LayerStack::HandleEvent(shared::event::Event &event)
{
  auto layer_event = dynamic_cast<shared::event::layer::LayerEvent &>(event);
  switch (layer_event.GetAction()) {
  case shared::event::layer::Action::Pop: {
    auto action = [this]() { this->PopLayer(); };
    command_queue_->Push(std::make_unique<command::FunctionalCommand>(action));
    break;
  }
  case shared::event::layer::Action::Push: {
    auto action = [this, layer_id = layer_event.GetLayerId()]() { this->PushLayerEvent(layer_id); };
    command_queue_->Push(std::make_unique<command::FunctionalCommand>(action));
    break;
  }
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

void LayerStack::PushLayerEvent(const LayerID layer_id)
{
  switch (layer_id) {
  case layer::LayerID::GameLayer: {
    auto game_state = std::make_shared<states::GameState>();
    auto game_layer = std::make_unique<layer::game::GameLayer>();
    game_layer->PushState(game_state);
    if (!layers_.empty()) { layers_.pop_back(); }
    PushLayer(std::move(game_layer));
    break;
  }
  case layer::LayerID::NetworkLayer: {
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

    if (!layers_.empty()) { layers_.pop_back(); }
    PushLayer(std::move(network_layer));
    PushLayer(std::move(game_layer));
  } break;
  }
}

}// namespace chroma::app::layer
