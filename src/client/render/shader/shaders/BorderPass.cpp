#include "chroma/client/render/shader/shaders/BorderPass.h"

#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/AngleMagPass.h"
#include "chroma/client/render/shader/shaders/GrayPass.h"
#include "chroma/client/render/shader/shaders/ThresholdPass.h"
#include "chroma/client/render/shader/shaders/Hysteresis.h"
#include "chroma/client/render/shader/shaders/NonMaximum.h"

#include <memory>
#include <raylib.h>
#include <rlgl.h>

namespace chroma::client::render::shader::shaders {
BorderPass::BorderPass(const int width, const int height)
  : ShaderPass("assets/shaders/base.vs", "assets/shaders/border.fs"),
    pass_angle_mag__(std::make_unique<AngleMagPass>(width, height)), pass_gray_(std::make_unique<GrayPass>()),
    pass_non_maximum_(std::make_unique<NonMaximum>()), pass_hysteresis_(std::make_unique<Hysteresis>()),
    pass_threshold_(std::make_unique<ThresholdPass>()),
    tex_angle_mag_(std::make_shared<RenderTexture2D>(LoadFloatRenderTexture(width, height))),
    tex_non_maximum_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    tex_hysteresis_ping(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    tex_hysteresis_pong(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    slot_angle_mag_val_(std::make_shared<int>(1)), slot_non_maximum_val_(std::make_shared<int>(2))
{
  SetPassType(PassType::BORDER);
  SetUniform("u_angle_mag", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, slot_angle_mag_val_);
  SetUniform("u_non_maximum", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, slot_non_maximum_val_);
}

BorderPass::~BorderPass() { UnloadRenderTexture(*tex_angle_mag_); }

void BorderPass::Setup()
{
  pass_gray_->Setup();
  pass_angle_mag__->Setup();
  pass_non_maximum_->Setup();
  pass_hysteresis_->Setup();
  pass_threshold_->Setup();

  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void BorderPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  pass_gray_->Execute(src, dst);
  pass_angle_mag__->Execute(dst, *tex_angle_mag_);
  // pass_non_maximum_->Execute(*tex_angle_mag_, *tex_non_maximum_);
  pass_non_maximum_->Execute(*tex_angle_mag_, *tex_hysteresis_ping);

  for (int i = 0; i < 5; ++i) {
    if (i % 2 == 0) {
      pass_hysteresis_->Execute(*tex_hysteresis_ping, *tex_hysteresis_pong);
    } else {
      pass_hysteresis_->Execute(*tex_hysteresis_pong, *tex_hysteresis_ping);
    }
  }
  pass_threshold_->Execute(*((10 % 2 == 0) ? tex_hysteresis_ping : tex_hysteresis_pong), *tex_non_maximum_);

  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);
  UploadAll();

  rlActiveTextureSlot(1);
  rlEnableTexture(tex_angle_mag_->texture.id);

  rlActiveTextureSlot(2);
  rlEnableTexture(tex_non_maximum_->texture.id);

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
