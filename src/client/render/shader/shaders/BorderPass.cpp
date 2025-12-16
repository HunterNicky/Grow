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
#include <utility>
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
    tex_composite_(std::make_shared<RenderTexture2D>(LoadRenderTexture(width, height))),
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

BorderPass::~BorderPass() 
{ 
  UnloadRenderTexture(*tex_angle_mag_); 
  UnloadRenderTexture(*tex_composite_);
  UnloadShader(composite_shader_);
}

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

  composite_shader_ = ::LoadShader(nullptr, "assets/shaders/border_composite.fs");

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }

  pass_noise_->SetSeed(static_cast<float>(GetRandomValue(0, 100000)) * 0.01F);
}

void BorderPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  RenderTexture2D* current_src = &src;
  
  if (gray_enabled_) {
    pass_gray_->Execute(*current_src, *ping_);
    current_src = ping_.get();
  }

  if (blur_enabled_) {
    pass_blur_->SetDirection(Vector2{ 1.0F, 0.0F });
    pass_blur_->Execute(*current_src, *pong_);
    pass_blur_->SetDirection(Vector2{ 0.0F, 1.0F });
    pass_blur_->Execute(*pong_, *ping_);
    current_src = ping_.get();
  }

  if (angle_mag_enabled_) {
    pass_angle_mag__->Execute(*current_src, *tex_angle_mag_);
    current_src = tex_angle_mag_.get();
  }

  if (non_maximum_enabled_) {
    pass_non_maximum_->Execute(*current_src, *ping_);
    current_src = ping_.get();
  }

  if (hysteresis_enabled_) {
    for (int i = 0; i < hysteresis_iterations_; ++i) {
      pass_hysteresis_->Execute(*current_src, *pong_);
      std::swap(ping_, pong_);
      current_src = ping_.get();
    }
  }

  if (threshold_enabled_) {
    pass_threshold_->Execute(*current_src, *pong_);
    current_src = pong_.get();
  }

  if (dilatation_enabled_) {
    pass_dilatation_->Execute(*current_src, *tex_dilated_);
  } else {
    BeginTextureMode(*tex_dilated_);
    DrawTextureRec(current_src->texture,
      Rectangle{ 0, 0, static_cast<float>(current_src->texture.width), static_cast<float>(-current_src->texture.height) },
      Vector2{ 0, 0 },
      WHITE);
    EndTextureMode();
  }

  if (noise_enabled_) {
    pass_noise_->Execute(*tex_dilated_, *tex_noise_);
  } else {
    BeginTextureMode(*tex_noise_);
    ClearBackground(BLACK);
    EndTextureMode();
  }

  BeginTextureMode(dst);
  ClearBackground(BLANK);

  if (composite_with_original_) {
    BeginTextureMode(*tex_composite_);
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

    BeginShaderMode(composite_shader_);
    
    const int loc_original = GetShaderLocation(composite_shader_, "u_original");
    const int loc_borders = GetShaderLocation(composite_shader_, "u_borders");
    
    int slot_original = 1;
    int slot_borders = 2;
    
    rlActiveTextureSlot(1);
    rlEnableTexture(src.texture.id);
    SetShaderValue(composite_shader_, loc_original, &slot_original, SHADER_UNIFORM_INT);

    rlActiveTextureSlot(2);
    rlEnableTexture(tex_composite_->texture.id);
    SetShaderValue(composite_shader_, loc_borders, &slot_borders, SHADER_UNIFORM_INT);

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
  } else {
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
  }

  EndTextureMode();
}

}// namespace chroma::client::render::shader::shaders
