#pragma once

#include "chroma/client/render/shader/ShaderPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class NoisePass : public ShaderPass
{
public:
  explicit NoisePass(int width = 1920, int height = 1080);
  ~NoisePass() override = default;

  NoisePass(const NoisePass &) = delete;
  NoisePass &operator=(const NoisePass &) = delete;
  NoisePass(NoisePass &&) noexcept = default;
  NoisePass &operator=(NoisePass &&) noexcept = default;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

  void SetSeed(float seed);

private:
  std::shared_ptr<float> seed_;
};

}// namespace chroma::client::render::shader::shaders
