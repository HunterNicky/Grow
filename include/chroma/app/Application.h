#pragma once

#include <memory>

#include "chroma/app/layers/LayerStack.h"
#include "chroma/client/audio/AudioEngine.h"
#include "chroma/app/database/DatabaseManager.h"
#include "chroma/client/render/Renderer.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app {
class Application
{
public:
  Application();
  Application(const Application &) = delete;
  Application(Application &&) = delete;
  Application &operator=(const Application &) = delete;
  Application &operator=(Application &&) = delete;
  ~Application() = default;

  static void Initialize();
  void Run();
  static void Shutdown();

  void PushLayer(std::unique_ptr<layer::Layer> layer) const;
  void PopLayer() const;
  void PushOverlay(std::unique_ptr<layer::Layer> overlay) const;
  void PopOverlay() const;

  static void DispatchEvent(shared::event::Event &event);

  void OnEvent(shared::event::Event &event);

private:
  std::unique_ptr<layer::LayerStack> layer_stack_;
  std::shared_ptr<shared::event::EventCatcher> event_catcher_;

  float delta_time_;
  std::shared_ptr<client::render::Renderer> renderer_;
  std::unique_ptr<client::audio::AudioEngine> audio_;
  std::unique_ptr<app::database::DatabaseManager> database_;

  shared::event::Subscription layer_subscription_;
};
}// namespace chroma::app
