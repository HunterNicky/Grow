#pragma once

#include <raylib.h>
#include <vector>

#include "chroma/client/render/shader/ShaderData.h"

namespace chroma::client::render {
class RenderTarget
{
public:
  RenderTarget(int width, int height);
  ~RenderTarget();

  void Begin() const;
  static void End();
  void Draw(int screen_width, int screen_height);
  void Draw(const Texture2D& texture, int screen_width, int screen_height);
  void RunPostProcessPipeline(const std::vector<shader::ShaderData*>& active_shaders);

  [[nodiscard]] int GetWidth() const { return width_; }
  [[nodiscard]] int GetHeight() const { return height_; }
  [[nodiscard]] RenderTexture2D GetTexture() const { return target_; }

private:
  RenderTexture2D target_;
  int width_;
  int height_;
  Rectangle source_rec_;
  Rectangle dest_rec_;

  void CalculateLetterbox(int screen_width, int screen_height);
  
  RenderTexture2D post_process_buffer_a_;
  RenderTexture2D post_process_buffer_b_;
};
}// namespace chroma::client::render
