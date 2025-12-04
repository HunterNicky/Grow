#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class NonMaximum : public ShaderPass
{
public:
  NonMaximum();

  void Setup();
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;
};
}// namespace chroma::client::render::shader::shaders