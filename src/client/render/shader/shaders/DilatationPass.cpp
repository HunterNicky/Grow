#include "chroma/client/render/shader/shaders/DilatationPass.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <raylib.h>

namespace chroma::client::render::shader::shaders {

DilatationPass::DilatationPass(const int width, const int height)
  : ShaderPass("assets/shaders/base.vs", "assets/shaders/dilatation.fs")
{
  (void)width;
  (void)height;
  SetPassType(PassType::DILATATION);
}

void DilatationPass::Setup() { LoadShader(); }

void DilatationPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);

  DrawTextureRec(src.texture,
    Rectangle{ 0, 0, static_cast<float>(src.texture.width), static_cast<float>(-src.texture.height) },
    Vector2{ 0, 0 },
    WHITE);

  EndShaderMode();
  EndTextureMode();
}

}// namespace chroma::client::render::shader::shaders
