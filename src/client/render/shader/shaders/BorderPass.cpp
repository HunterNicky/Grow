#include "chroma/client/render/shader/shaders/BorderPass.h"

#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/AngleMagPass.h"
#include "chroma/client/render/shader/shaders/BlurPass.h"
#include "chroma/client/render/shader/shaders/GrayPass.h"
#include "chroma/client/render/shader/shaders/Hysteresis.h"
#include "chroma/client/render/shader/shaders/NonMaximum.h"
#include "chroma/client/render/shader/shaders/ThresholdPass.h"

#include <memory>
#include <raylib.h>
#include <rlgl.h>

namespace chroma::client::render::shader::shaders {
BorderPass::BorderPass(const int width, const int height)
  : ShaderPass("assets/shaders/base.vs", "assets/shaders/border.fs"),
    pass_angle_mag__(std::make_unique<AngleMagPass>(width, height)), pass_gray_(std::make_unique<GrayPass>()),
    pass_non_maximum_(std::make_unique<NonMaximum>()), pass_hysteresis_(std::make_unique<Hysteresis>()),
    pass_blur_(std::make_unique<BlurPass>(width, height)), pass_threshold_(std::make_unique<ThresholdPass>()),
    pass_dilatation_(std::make_unique<DilatationPass>(width, height)),
    pass_noise_(std::make_unique<NoisePass>(width, height)),
    tex_angle_mag_(std::make_shared<RenderTexture2D>(LoadFloatRenderTexture(width, height))),
    tex_non_maximum_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    tex_dilated_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    tex_noise_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    ping_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    pong_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
    slot_edges_(std::make_shared<int>(1)),
    slot_noise_val_(std::make_shared<int>(2)), spread_val_(std::make_shared<float>(0.005F)),
    resolution_(std::make_shared<Vector2>(static_cast<float>(width), static_cast<float>(height)))
{
  SetPassType(PassType::BORDER);
  SetUniform("u_edges", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, slot_edges_);
  SetUniform("u_noise", rlShaderUniformDataType::RL_SHADER_UNIFORM_INT, slot_noise_val_);
  SetUniform("u_resolution", rlShaderUniformDataType::RL_SHADER_UNIFORM_VEC2, resolution_);
  SetUniform("u_spread", rlShaderUniformDataType::RL_SHADER_UNIFORM_FLOAT, spread_val_);
}

BorderPass::~BorderPass() { UnloadRenderTexture(*tex_angle_mag_); }

void BorderPass::Setup()
{
  pass_gray_->Setup();
  pass_angle_mag__->Setup();
  pass_non_maximum_->Setup();
  pass_hysteresis_->Setup();
  pass_threshold_->Setup();
  pass_blur_->Setup();
  pass_dilatation_->Setup();
  pass_noise_->Setup();

  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }

  pass_noise_->SetSeed(static_cast<float>(GetRandomValue(0, 100000)) * 0.01F);
}

void BorderPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  pass_gray_->Execute(src, *ping_);

  pass_blur_->SetDirection(Vector2{ 1.0F, 0.0F });
  pass_blur_->Execute(*ping_, *pong_);
  pass_blur_->SetDirection(Vector2{ 0.0F, 1.0F });
  pass_blur_->Execute(*pong_, *ping_);

  pass_angle_mag__->Execute(*ping_, *tex_angle_mag_);

  pass_non_maximum_->Execute(*tex_angle_mag_, *ping_);

  for (int i = 0; i < 5; ++i) {
    pass_hysteresis_->Execute(*ping_, *pong_);
    std::swap(ping_, pong_);
  }
  pass_threshold_->Execute(*ping_, *pong_);

  pass_dilatation_->SetRadius(3);
  pass_dilatation_->Execute(*pong_, *tex_dilated_);

  pass_noise_->Execute(*tex_dilated_, *tex_noise_);

  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);
  UploadAll();

  rlActiveTextureSlot(1);
  rlEnableTexture(tex_dilated_->texture.id);

  rlActiveTextureSlot(2);
  rlEnableTexture(tex_noise_->texture.id);

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
