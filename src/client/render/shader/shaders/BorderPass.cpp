#include "chroma/client/render/shader/shaders/BorderPass.h"

#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/DerivativeXPass.h"
#include "chroma/client/render/shader/shaders/DerivativeYPass.h"
#include "rlgl.h"

#include <memory>
#include <print>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {
RenderTexture2D LoadFloatRenderTexture(const int width, const int height)
{
  RenderTexture2D target = { 0 };

  target.id = rlLoadFramebuffer();
  rlEnableFramebuffer(target.id);
  if (target.id > 0) {
    target.texture.id = rlLoadTexture(nullptr, width, height, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
    target.texture.width = width;
    target.texture.height = height;
    target.texture.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
    target.texture.mipmaps = 1;

    rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);

    if (rlFramebufferComplete(target.id)) {
      TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);
    } else {
      TraceLog(LOG_WARNING, "FBO: [ID %i] Framebuffer object not complete", target.id);
    }
  } else {
    TraceLog(LOG_WARNING, "FBO: Framebuffer object can not be created");
  }

  return target;
}

BorderPass::BorderPass(const int width, const int height)
  : ShaderPass("resources/shaders/base.vs", "assets/shaders/border.fs"), pass_x_(std::make_unique<DerivativeXPass>()),
    pass_y_(std::make_unique<DerivativeYPass>()),
    tex_dx_(std::make_shared<RenderTexture2D>(LoadFloatRenderTexture(width, height))),
    tex_dy_(std::make_shared<RenderTexture2D>(LoadFloatRenderTexture(width, height)))
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

  std::print("DX format: {}\n DY format: {}\n", tex_dx_->texture.format, tex_dy_->texture.format);

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

BorderPass::~BorderPass()
{
  UnloadRenderTexture(*tex_dx_);
  UnloadRenderTexture(*tex_dy_);
}

}// namespace chroma::client::render::shader::shaders
