#pragma once

#include <memory>

#include "chroma/app/layers/LayerStack.h"
#include "chroma/client/core/Window.h"
#include "chroma/shared/events/EventCatcher.h"
#include "chroma/shared/events/EventDispatcher.h"

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

  void DispatchEvent(shared::event::Event &event) const;

private:
  std::unique_ptr<layer::LayerStack> layer_stack_;
  std::shared_ptr<shared::event::EventDispatcher> event_dispatcher_;
  std::shared_ptr<shared::event::EventCatcher> event_catcher_;

  float delta_time_;

  client::core::Window window_;
};
}// namespace chroma::app
