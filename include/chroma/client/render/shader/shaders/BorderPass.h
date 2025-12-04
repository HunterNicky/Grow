#pragma once

#include "chroma/client/render/shader/shaders/DerivativeXPass.h"
#include "chroma/client/render/shader/shaders/DerivativeYPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include <raylib.h>
#include <memory>

namespace chroma::client::render::shader::shaders {

class BorderPass : public ShaderPass
{
public:
  explicit BorderPass(int width = 1920, int height = 1080);
  ~BorderPass() override;

  BorderPass(const BorderPass &) = delete;
  BorderPass &operator=(const BorderPass &) = delete;     
  BorderPass(BorderPass &&) noexcept = default;
  BorderPass &operator=(BorderPass &&) noexcept = default;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

private:

  std::unique_ptr<DerivativeXPass> pass_x_;
  std::unique_ptr<DerivativeYPass> pass_y_;

  std::shared_ptr<RenderTexture2D> tex_dx_;
  std::shared_ptr<RenderTexture2D> tex_dy_;
};

} // namespace chroma::client::render::shader::shaders
