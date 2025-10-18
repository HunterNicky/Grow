#define RAYGUI_IMPLEMENTATION

#include <raylib.h>
#include <memory>
#include <raygui.h>
#include <utility>
#include <thread>

#include "chroma/app/Application.h"
#include "chroma/app/events/Event.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/LayerStack.h"
#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/states/NetworkState.h"

namespace chroma::app {
Application::Application() : layer_stack_(std::make_unique<layer::LayerStack>()), delta_time_(0.F), window_(1280, 720, "Chroma") {};

void Application::Run()
{
    window_.Init();

    auto game_layer = std::make_unique<layer::game::GameLayer>();
    game_layer->PushState(std::make_shared<layer::states::GameState>());

    auto network_layer = std::make_unique<layer::network::NetworkLayer>();
    network_layer->PushState(std::make_shared<layer::states::NetworkState>());

    PushLayer(std::move(game_layer));
    PushLayer(std::move(network_layer));

    auto last_time = static_cast<float>(GetTime());

    while (!client::core::Window::ShouldClose()) {
      
      const auto current_time = static_cast<float>(GetTime());
      delta_time_ = current_time - last_time;
      last_time = current_time;

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

void Application::DispatchEvent(event::Event &event) const { event_dispatcher_->Dispatch(event); }
}// namespace chroma::app
