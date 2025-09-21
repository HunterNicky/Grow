#pragma once

#include <raylib.h>
#include <string>

namespace chroma::client::core {
using WindowSize = struct WindowSize
{
  int width = 0;
  int height = 0;
};

class Window
{
public:
  Window(int width, int height, const char *title);
  Window(const Window &) = default;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = delete;
  ~Window() = default;

  void FullScreen();
  void Resize(int width, int height);

  [[nodiscard]] bool static ShouldClose();
  void static Close();
  void Init() const;

private:
  void FixMouseScale() const;

  WindowSize actual_window_size_;
  WindowSize monitor_size_;
  WindowSize virtual_size_;
  std::string title_{ "Chroma" };
};
}// namespace chroma::client::core
