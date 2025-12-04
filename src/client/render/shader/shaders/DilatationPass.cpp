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
  radius_ = std::make_shared<int>(3);
  SetUniform("u_radius", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, radius_);
}

void DilatationPass::Setup()
{
  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}
void DilatationPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);
  UploadAll();
  DrawTextureRec(src.texture,
    Rectangle{ 0, 0, static_cast<float>(src.texture.width), static_cast<float>(-src.texture.height) },
    Vector2{ 0, 0 },
    WHITE);

  EndShaderMode();
  EndTextureMode();
}

void DilatationPass::SetRadius(const int radius) const { *radius_ = radius; }

}// namespace chroma::client::render::shader::shaders
