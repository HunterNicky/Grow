#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/DerivativeXPass.h"
#include "chroma/client/render/shader/shaders/DerivativeYPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

BorderPass::BorderPass(const int width, const int height)
  : ShaderPass("resources/shaders/base.vs", "assets/shaders/border.fs"), pass_x_(std::make_unique<DerivativeXPass>()),
    pass_y_(std::make_unique<DerivativeYPass>()),
    tex_dx_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    tex_dy_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height)))
{
  SetPassType(PassType::BORDER);
  SetUniform("u_dx", UniformType::SAMPLER2D, tex_dx_);
  SetUniform("u_dy", UniformType::SAMPLER2D, tex_dy_);
}

void BorderPass::Setup()
{
  pass_x_->Setup();
  pass_y_->Setup();
  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void BorderPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  pass_x_->Execute(src, *tex_dx_);
  pass_y_->Execute(src, *tex_dy_);

  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);
  UploadAll();

  DrawTextureRec(
    src.texture, Rectangle{ 0, 0, (float)src.texture.width, (float)-src.texture.height }, Vector2{ 0, 0 }, BLACK);

  EndShaderMode();
  EndTextureMode();
}

BorderPass::~BorderPass()
{
  UnloadRenderTexture(*tex_dx_);
  UnloadRenderTexture(*tex_dy_);
}

}// namespace chroma::client::render::shader::shaders
