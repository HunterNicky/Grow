#include <raylib.h>

#include "chroma/client/core/Window.h"

namespace chroma::client::core {
Window::Window(int width, int height, const char *title)
  : actual_window_size_({ .width = width, .height = height }), monitor_size_(), virtual_size_(actual_window_size_),
    title_(title)
{
  const int monitor = GetCurrentMonitor();
  monitor_size_.height = GetMonitorHeight(monitor);
  monitor_size_.width = GetMonitorWidth(monitor);
}

void Window::FullScreen()
{
  if (IsWindowFullscreen()) {
    Resize(virtual_size_.width, virtual_size_.height);
    ToggleFullscreen();
    return;
  }

  const int monitor = GetCurrentMonitor();
  monitor_size_.height = GetMonitorHeight(monitor);
  monitor_size_.width = GetMonitorWidth(monitor);

  Resize(monitor_size_.width, monitor_size_.height);
  ToggleFullscreen();
}

void Window::Close() { CloseWindow(); }

bool Window::ShouldClose() { return WindowShouldClose(); }

void Window::Init() const { InitWindow(actual_window_size_.width, actual_window_size_.height, title_.c_str()); }

void Window::Resize(const int width, const int height)
{
  actual_window_size_.width = width;
  actual_window_size_.height = height;
  SetWindowSize(actual_window_size_.width, actual_window_size_.height);
  FixMouseScale();
}

void Window::FixMouseScale() const
{
  const float scaleX = static_cast<float>(virtual_size_.width) / static_cast<float>(actual_window_size_.width);
  const float scaleY = static_cast<float>(virtual_size_.height) / static_cast<float>(actual_window_size_.height);
  SetMouseScale(scaleX, scaleY);
}
}// namespace chroma::client::core
