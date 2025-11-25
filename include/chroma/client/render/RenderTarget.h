#pragma once

#include <raylib.h>

namespace chroma::client::render {
class RenderTarget
{
public:
  RenderTarget(int width, int height);
  ~RenderTarget();

  void Begin() const;
  static void End();
  void Draw(int screen_width, int screen_height);
  void Draw(const Texture2D &texture, int screen_width, int screen_height);

  [[nodiscard]] int GetWidth() const { return width_; }
  [[nodiscard]] int GetHeight() const { return height_; }
  [[nodiscard]] RenderTexture2D GetTexture() const { return target_; }
  void SetRenderTexture(const RenderTexture2D &rt) { target_ = rt; }

private:
  RenderTexture2D target_;
  int width_;
  int height_;
  Rectangle source_rec_;
  Rectangle dest_rec_;

  void CalculateLetterbox(int screen_width, int screen_height);
};
}// namespace chroma::client::render
