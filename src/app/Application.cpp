#define RAYGUI_IMPLEMENTATION

#include <memory>
#include <raygui.h>
#include <raylib.h>
#include <utility>

#include "chroma/app/Application.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/LayerStack.h"
#include "chroma/app/layers/game/GameLayer.h"
#include "chroma/app/layers/network/NetworkLayer.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/client/audio/AudioBridgeImpl.h"
#include "chroma/client/audio/AudioEngine.h"
#include "chroma/client/render/RenderBridgeImpl.h"
#include "chroma/client/render/Renderer.h"
#include "chroma/client/render/Window.h"
#include "chroma/shared/audio/AudioBridge.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/render/RenderBridge.h"

namespace chroma::app {

Application::Application()
  : layer_stack_(std::make_unique<layer::LayerStack>()), delta_time_(0.F), renderer_([&] {
      const client::render::RenderConfig config{};
      auto window = std::make_unique<client::render::Window>(1280, 720, "Chroma");
      return std::make_unique<client::render::Renderer>(std::move(window), config);
    }()),
    audio_(std::make_unique<client::audio::AudioEngine>())
{
  const auto bridge = std::make_shared<client::render::RenderBridgeImpl>(renderer_.get());
  shared::render::SetRenderBridge(bridge);

  const auto audio_bridge = std::make_shared<client::audio::AudioBridgeImpl>(audio_.get());
  shared::audio::SetAudioBridge(audio_bridge);

  audio_bridge->LoadSound("step", "assets/sfx/step.wav");
  audio_bridge->LoadMusic("bgm", "assets/music/06.mp3");
  audio_bridge->PlayMusic("bgm", true, 0.4F);
}

void Application::Run()
{
  Initialize();
  {
    auto event_dispatcher = std::make_unique<shared::event::EventDispatcher>();
    shared::event::EventBus::SetDispatcher(event_dispatcher);
  }

  auto game_layer = std::make_unique<layer::game::GameLayer>();
  auto network_layer = std::make_unique<layer::network::NetworkLayer>();

  auto mediator = std::make_shared<states::GameNetworkMediator>();
  const auto game_state = std::make_shared<states::GameState>(mediator);
  game_state->SetEventDispatcher();
  const auto network_state = std::make_shared<states::NetworkState>(mediator);
  network_state->SetEventDispatcher();

  mediator->SetGameState(game_state);
  mediator->SetNetworkState(network_state);

  // auto game_state = std::make_shared<states::GameState>();

  network_layer->PushState(network_state);
  game_layer->PushState(game_state);

  PushLayer(std::move(network_layer));
  PushLayer(std::move(game_layer));

  event_catcher_ = std::make_shared<shared::event::EventCatcher>();

  auto last_time = static_cast<float>(GetTime());

  while (!renderer_->ShouldClose()) {
    const auto current_time = static_cast<float>(GetTime());
    delta_time_ = current_time - last_time;
    last_time = current_time;

    event_catcher_->CatchEvent();

    if (const auto ab = shared::audio::GetAudioBridge()) { ab->Update(); }

    layer_stack_->UpdateLayers(delta_time_);

    renderer_->RenderFrame([&] { layer_stack_->RenderLayers(); });
  }
  renderer_->Close();
}

void Application::Initialize() { shared::render::GetRenderBridge()->LoadSprite("assets/sprites/player/randi-1.png"); }

void Application::Shutdown() {}

void Application::PushLayer(std::unique_ptr<layer::Layer> layer) const { layer_stack_->PushLayer(std::move(layer)); }

void Application::PopLayer() const { layer_stack_->PopLayer(); }

void Application::PushOverlay(std::unique_ptr<layer::Layer> overlay) const
{
  layer_stack_->PushOverlay(std::move(overlay));
}

void Application::PopOverlay() const { layer_stack_->PopOverlay(); }

void Application::DispatchEvent(shared::event::Event &event)
{
  shared::event::EventBus::GetDispatcher()->Dispatch(event);
}
}// namespace chroma::app
