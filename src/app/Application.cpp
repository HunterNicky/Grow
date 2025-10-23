#define RAYGUI_IMPLEMENTATION

#include <raylib.h>
#include <memory>
#include <raygui.h>
#include <utility>

#include "chroma/app/Application.h"
#include "chroma/shared/events/Event.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/LayerStack.h"
#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/states/NetworkState.h"
#include "chroma/shared/events/EventCatcher.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"

namespace chroma::app {
Application::Application() : layer_stack_(std::make_unique<layer::LayerStack>()), delta_time_(0.F), window_(1280, 720, "Chroma") {};

void Application::Run()
{
    window_.Init();

    auto game_layer = std::make_unique<layer::game::GameLayer>();
    auto network_layer = std::make_unique<layer::network::NetworkLayer>();


    states::GameNetworkMediator mediator = states::GameNetworkMediator();
    
    auto game_state = std::make_shared<states::GameState>(std::make_shared<states::GameNetworkMediator>(mediator));
    auto network_state = std::make_shared<states::NetworkState>(std::make_shared<states::GameNetworkMediator>(mediator));
    
    mediator.SetGameState(game_state);
    mediator.SetNetworkState(network_state);

    game_layer->PushState(game_state);
    network_layer->PushState(network_state);

    PushLayer(std::move(network_layer));
    PushLayer(std::move(game_layer));

    auto last_time = static_cast<float>(GetTime());

    while (!client::core::Window::ShouldClose()) {
      
      const auto current_time = static_cast<float>(GetTime());
      delta_time_ = current_time - last_time;
      last_time = current_time;

      auto event_ptr = shared::event::EventCatcher::CatchEvent();

      if (event_ptr) {
         layer_stack_->HandleEvent(*event_ptr);
      }

      layer_stack_->UpdateLayers(delta_time_);

      BeginDrawing();
      ClearBackground(RAYWHITE);

      layer_stack_->RenderLayers();

      EndDrawing();
  }

  client::core::Window::Close();
}

void Application::Initialize() {}

void Application::Shutdown() {}

void Application::PushLayer(std::unique_ptr<layer::Layer> layer) const { layer_stack_->PushLayer(std::move(layer)); }

void Application::PopLayer() const { layer_stack_->PopLayer(); }

void Application::PushOverlay(std::unique_ptr<layer::Layer> overlay) const
{
  layer_stack_->PushOverlay(std::move(overlay));
}

void Application::PopOverlay() const { layer_stack_->PopOverlay(); }

void Application::DispatchEvent(shared::event::Event &event) const { event_dispatcher_->Dispatch(event); }
}// namespace chroma::app
