#pragma once

#include "chroma/client/render/shader/ShaderPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class DilatationPass : public ShaderPass
{
public:
  explicit DilatationPass(int width = 1920, int height = 1080);
  ~DilatationPass() override = default;

  DilatationPass(const DilatationPass &) = delete;
  DilatationPass &operator=(const DilatationPass &) = delete;
  DilatationPass(DilatationPass &&) noexcept = default;
  DilatationPass &operator=(DilatationPass &&) noexcept = default;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;
};

}// namespace chroma::client::render::shader::shaders
