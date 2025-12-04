#pragma once

#include "chroma/client/render/shader/ShaderPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class DilatationPass final : public ShaderPass
{
public:
  explicit DilatationPass(int width = 1920, int height = 1080);
  ~DilatationPass() override = default;

  DilatationPass(const DilatationPass &) = delete;
  DilatationPass &operator=(const DilatationPass &) = delete;
  DilatationPass(DilatationPass &&) noexcept = delete;
  DilatationPass &operator=(DilatationPass &&) noexcept = delete;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

  void SetRadius(int radius) const;

private:
  std::shared_ptr<int> radius_;
};

}// namespace chroma::client::render::shader::shaders
