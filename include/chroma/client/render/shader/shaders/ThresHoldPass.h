#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class ThresHoldPass : public ShaderPass
{
public:
  explicit ThresHoldPass(float threshold);

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

  [[nodiscard]] std::shared_ptr<float> GetThreshold() const;
  void SetThreshold(float threshold);

private:
    std::shared_ptr<float> threshold_;
};

} // namespace chroma::client::render::shader::shaders
