#pragma once

#include "Window.h"

namespace chroma::client::core {
class Application
{
public:
  Application();
  Application(const Application &) = default;
  Application(Application &&) = delete;
  Application &operator=(const Application &) = default;
  Application &operator=(Application &&) = delete;
  ~Application() = default;

  void run();

private:
  Window window_;
};
}// namespace chroma::client::core
