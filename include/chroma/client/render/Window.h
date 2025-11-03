#pragma once

#include <raylib.h>
#include <string>

namespace chroma::client::render {
using WindowSize = struct WindowSize
{
  int width = 0;
  int height = 0;
};

struct RenderConfig
{
  int virtual_width = 1920;
  int virtual_height = 1080;
  bool vsync = true;
  int target_fps = 60;
  TextureFilter filter = TEXTURE_FILTER_BILINEAR;
};

enum class DisplayMode : uint8_t { Windowed = 0, Fullscreen = 1, BorderlessFullscreen = 2 };

class Window
{
public:
  Window(int width, int height, const char *title);
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;
  ~Window();

  void Update();

  void SetDisplayMode(DisplayMode mode);
  void SetVirtualResolution(int width, int height);
  static void SetMinimumSize(int width, int height);
  [[nodiscard]] WindowSize GetVirtualSize() const;
  [[nodiscard]] float GetAspectRatio() const;

  void Resize(int width, int height);
  static void Maximize();
  static void Restore();

  [[nodiscard]] bool static ShouldClose();
  void static Close();

private:
  void FixMouseScale() const;

  void FullScreen();
  void Borderless();
  void Windowed();

  DisplayMode display_mode_;
  WindowSize actual_window_size_;
  WindowSize monitor_size_;
  WindowSize virtual_size_;
  std::string title_{ "Chroma" };

  WindowSize windowed_size_;
  Vector2 windowed_pos_ = { 0, 0 };
};
}// namespace chroma::client::render