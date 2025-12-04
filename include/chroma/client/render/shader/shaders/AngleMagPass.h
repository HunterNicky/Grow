#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/DerivativeXPass.h"
#include "chroma/client/render/shader/shaders/DerivativeYPass.h"
#include "chroma/client/render/shader/shaders/GrayPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {
class AngleMagPass : public ShaderPass
{
public:
  explicit AngleMagPass(int width = 1920, int height = 1080);
  ~AngleMagPass() override;

  AngleMagPass(const AngleMagPass &) = delete;
  AngleMagPass &operator=(const AngleMagPass &) = delete;
  AngleMagPass(AngleMagPass &&) noexcept = default;
  AngleMagPass &operator=(AngleMagPass &&) noexcept = default;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

private:
  std::unique_ptr<DerivativeXPass> derivative_x_pass_;
  std::unique_ptr<DerivativeYPass> derivative_y_pass_;

  std::shared_ptr<RenderTexture2D> tex_dx_;
  std::shared_ptr<RenderTexture2D> tex_dy_;

  std::shared_ptr<int> slot_dx_val_;
  std::shared_ptr<int> slot_dy_val_;
};
}// namespace chroma::client::render::shader::shaders