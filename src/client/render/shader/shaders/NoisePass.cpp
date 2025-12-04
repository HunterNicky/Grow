#include "chroma/client/render/shader/shaders/NoisePass.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <raylib.h>
#include <memory>
#include <rlgl.h>

namespace chroma::client::render::shader::shaders {

NoisePass::NoisePass(const int width, const int height)
  : ShaderPass("assets/shaders/base.vs", "assets/shaders/noise.fs"), seed_(std::make_shared<float>(0.0f))
{
  (void)width;
  (void)height;
  SetPassType(PassType::NOISE);
  SetUniform("u_seed", rlShaderUniformDataType::RL_SHADER_UNIFORM_FLOAT, seed_);
}

void NoisePass::Setup() { LoadShader(); }

void NoisePass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  (void)src;
  BeginTextureMode(dst);
  ClearBackground(BLACK);

  BeginShaderMode(shader_);
  UploadAll();

  DrawRectangle(0, 0, dst.texture.width, dst.texture.height, WHITE);

  EndShaderMode();
  EndTextureMode();
}

void NoisePass::SetSeed(const float seed) { *seed_ = seed; }

}// namespace chroma::client::render::shader::shaders
