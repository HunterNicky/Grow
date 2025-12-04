#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class BorderColorPass : public ShaderPass
{
public:
  BorderColorPass();

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

};
} // namespace chroma::client::render::shader::shaders
