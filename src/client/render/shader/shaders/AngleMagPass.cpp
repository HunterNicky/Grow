#include "chroma/client/render/shader/shaders/AngleMagPass.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/DerivativeXPass.h"
#include "chroma/client/render/shader/shaders/DerivativeYPass.h"

#include <memory>
#include <raylib.h>
#include <rlgl.h>

namespace chroma::client::render::shader::shaders {
AngleMagPass::AngleMagPass(const int width, const int height)
  : ShaderPass("assets/shaders/base.vs", "assets/shaders/angle_mag.fs"),
    derivative_x_pass_(std::make_unique<DerivativeXPass>()), derivative_y_pass_(std::make_unique<DerivativeYPass>()),
    tex_dx_(std::make_shared<RenderTexture2D>(LoadFloatRenderTexture(width, height))),
    tex_dy_(std::make_shared<RenderTexture2D>(LoadFloatRenderTexture(width, height))),
    slot_dx_val_(std::make_shared<int>(1)), slot_dy_val_(std::make_shared<int>(2))
{
  SetPassType(PassType::ANGLE_MAG);
  SetUniform("u_dx", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, slot_dx_val_);
  SetUniform("u_dy", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, slot_dy_val_);
}

AngleMagPass::~AngleMagPass()
{
  UnloadRenderTexture(*tex_dx_);
  UnloadRenderTexture(*tex_dy_);
}

void AngleMagPass::Setup()
{
  derivative_x_pass_->Setup();
  derivative_y_pass_->Setup();
  LoadShader();
  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void AngleMagPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  derivative_x_pass_->Execute(src, *tex_dx_);
  derivative_y_pass_->Execute(src, *tex_dy_);

  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);
  UploadAll();

  rlActiveTextureSlot(1);
  rlEnableTexture(tex_dx_->texture.id);

  rlActiveTextureSlot(2);
  rlEnableTexture(tex_dy_->texture.id);

  rlActiveTextureSlot(0);

  DrawTextureRec(src.texture,
    Rectangle{ 0, 0, static_cast<float>(src.texture.width), static_cast<float>(-src.texture.height) },
    Vector2{ 0, 0 },
    WHITE);

  EndShaderMode();

  rlActiveTextureSlot(1);
  rlDisableTexture();
  rlActiveTextureSlot(2);
  rlDisableTexture();
  rlActiveTextureSlot(0);

  EndTextureMode();
}

}// namespace chroma::client::render::shader::shaders