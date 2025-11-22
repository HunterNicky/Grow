#include <raylib.h>

#include "chroma/client/render/Window.h"

namespace chroma::client::render {
Window::Window(const int width, const int height, const char *title)
  : display_mode_(DisplayMode::Windowed), actual_window_size_({ .width = width, .height = height }),
    virtual_size_({ .width = width, .height = height }), title_(title), windowed_size_(actual_window_size_)
{
  // NOLINTNEXTLINE
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

  if (!IsWindowReady()) { InitWindow(actual_window_size_.width, actual_window_size_.height, title_.c_str()); }

  const int monitor = GetCurrentMonitor();
  monitor_size_ = { .width = GetMonitorWidth(monitor), .height = GetMonitorHeight(monitor) };
  windowed_pos_ = GetWindowPosition();

  FixMouseScale();
}

Window::~Window()
{
  if (IsWindowReady()) { CloseWindow(); }
}

void Window::Update()
{
  if (IsWindowResized()) {
    actual_window_size_ = { .width = GetScreenWidth(), .height = GetScreenHeight() };
    FixMouseScale();

    if (display_mode_ == DisplayMode::Windowed) { windowed_size_ = actual_window_size_; }
  }
}

void Window::SetDisplayMode(const DisplayMode mode)
{
  if (display_mode_ == mode) { return; }

  const DisplayMode prev_mode = display_mode_;
  display_mode_ = mode;

  if (prev_mode == DisplayMode::Windowed) {
    windowed_size_ = { .width = GetScreenWidth(), .height = GetScreenHeight() };
    windowed_pos_ = GetWindowPosition();
  }

  switch (display_mode_) {
  case DisplayMode::Windowed:
    Windowed();
    break;
  case DisplayMode::Fullscreen:
    FullScreen();
    break;
  case DisplayMode::BorderlessFullscreen:
    Borderless();
    break;
  }
}

void Window::SetVirtualResolution(const int width, const int height)
{
  virtual_size_ = { .width = width, .height = height };
  FixMouseScale();
}

void Window::SetMinimumSize(const int width, const int height) { SetWindowMinSize(width, height); }

WindowSize Window::GetVirtualSize() const { return virtual_size_; }

float Window::GetAspectRatio() const
{
  return static_cast<float>(virtual_size_.width) / static_cast<float>(virtual_size_.height);
}

void Window::FullScreen()
{
  ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);

  if (!IsWindowFullscreen()) {
    const int monitor = GetCurrentMonitor();
    monitor_size_ = { .width = GetMonitorWidth(monitor), .height = GetMonitorHeight(monitor) };

    ToggleFullscreen();

    actual_window_size_ = monitor_size_;
    FixMouseScale();
  }
}

void Window::Borderless()
{
  if (IsWindowFullscreen()) { ToggleFullscreen(); }

  const int monitor = GetCurrentMonitor();
  monitor_size_ = { .width = GetMonitorWidth(monitor), .height = GetMonitorHeight(monitor) };

  SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
  Resize(monitor_size_.width, monitor_size_.height);
  SetWindowPosition(0, 0);
}

void Window::Windowed()
{
  if (IsWindowFullscreen()) { ToggleFullscreen(); }
  ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);

  Resize(windowed_size_.width, windowed_size_.height);
  SetWindowPosition(static_cast<int>(windowed_pos_.x), static_cast<int>(windowed_pos_.y));
}

void Window::Maximize() { MaximizeWindow(); }

void Window::Restore() { RestoreWindow(); }

void Window::Resize(const int width, const int height)
{
  actual_window_size_ = { .width = width, .height = height };
  SetWindowSize(width, height);
  FixMouseScale();
}

void Window::FixMouseScale() const
{
  const float scale_x = static_cast<float>(virtual_size_.width) / static_cast<float>(actual_window_size_.width);
  const float scale_y = static_cast<float>(virtual_size_.height) / static_cast<float>(actual_window_size_.height);
  SetMouseScale(scale_x, scale_y);
}

bool Window::ShouldClose() { return WindowShouldClose(); }

void Window::Close()
{
  if (IsWindowReady()) { CloseWindow(); }
}
}// namespace chroma::client::render