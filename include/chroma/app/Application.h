#pragma once

#include <memory>

#include "chroma/shared/events/EventDispatcher.h"
#include "chroma/app/layers/LayerStack.h"
#include "chroma/client/core/Window.h"

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
  std::unique_ptr<shared::event::EventDispatcher> event_dispatcher_;
  float delta_time_;

  client::core::Window window_;
};
}// namespace chroma::app
