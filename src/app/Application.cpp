#define RAYGUI_IMPLEMENTATION

#include <memory>
#include <raygui.h>
#include <raylib.h>
#include <utility>

#include "chroma/app/Application.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/app/layers/LayerStack.h"
// #include "chroma/app/layers/game/GameLayer.h"
// #include "chroma/app/layers/network/NetworkLayer.h"
// #include "chroma/app/states/GameState.h"
// #include "chroma/app/states/mediator/GameNetworkMediator.h"
// #include "chroma/app/states/network/NetworkState.h"
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

#include "chroma/app/layers/game/MainMenuLayer.h"
#include "chroma/client/ui/UIManager.h"
#include "chroma/client/ui/UIManagerBus.h"

#include "chroma/shared/events/layer/LayerEvent.h"

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
    auto ui_manager = std::make_unique<client::ui::UIManager>();
    client::ui::UIManagerBus::SetUIManager(ui_manager);
    layer_subscription_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::layer::LayerEvent>(
      [this](shared::event::Event &event) { this->OnEvent(event); });
  }

  auto menu_layer = std::make_unique<layer::game::MainMenuLayer>("MainMenu");
  PushLayer(std::move(menu_layer));

  event_catcher_ = std::make_shared<shared::event::EventCatcher>();

  auto last_time = static_cast<float>(GetTime());

  while (!renderer_->ShouldClose()) {
    const auto current_time = static_cast<float>(GetTime());
    delta_time_ = current_time - last_time;
    last_time = current_time;

    event_catcher_->CatchEvent();

    if (const auto ab = shared::audio::GetAudioBridge()) { ab->Update(); }

    layer_stack_->UpdateLayers(delta_time_);
    client::ui::UIManagerBus::GetUIManager()->OnUpdate(delta_time_);

    renderer_->RenderFrame([&] {
      layer_stack_->RenderLayers();
      client::ui::UIManagerBus::GetUIManager()->OnRender();
    });

    // BeginDrawing();
    // EndDrawing();
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

void Application::OnEvent(shared::event::Event &event)
{
  if (event.GetType() == shared::event::Event::Type::LayerEvent) { layer_stack_->HandleEvent(event); }
}
}// namespace chroma::app
