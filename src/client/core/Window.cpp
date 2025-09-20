#include "chroma/client/core/Window.h"
#include <raylib.h>

namespace chroma::client::core {
Window::Window(int width, int height, const char *title) : width_(width), height_(height), title_(title) {}

void Window::FullScreen() { ToggleFullscreen(); }

void Window::Close() { CloseWindow(); }

bool Window::ShouldClose() { return WindowShouldClose(); }

void Window::Init() { InitWindow(width_, height_, title_.c_str()); }

void Window::Resize(int width, int height)
{
  width_ = width;
  height_ = height;
  SetWindowSize(width_, height_);
}
}// namespace chroma::client::core
