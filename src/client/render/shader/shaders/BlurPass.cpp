#include "chroma/client/render/shader/shaders/BlurPass.h"
#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <algorithm>
#include <memory>
#include <raylib.h>
#include <rlgl.h>

namespace chroma::client::render::shader::shaders {

BlurPass::BlurPass(const int width, const int height, const Vector2 direction)
  : ShaderPass("assets/shaders/base.vs", "assets/shaders/blur.fs"),
    resolution_(std::make_shared<Vector2>(Vector2{ static_cast<float>(width), static_cast<float>(height) })),
    direction_(std::make_shared<Vector2>(direction))
{
  SetPassType(PassType::BLUR);
  SetUniform("u_resolution", rlShaderUniformDataType::RL_SHADER_UNIFORM_VEC2, resolution_);
  SetUniform("u_direction", rlShaderUniformDataType::RL_SHADER_UNIFORM_VEC2, direction_);
}

void BlurPass::SetDirection(const Vector2 &direction)
{
  direction_->x = direction.x;
  direction_->y = direction.y;
}

void BlurPass::Setup()
{
  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void BlurPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
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

}// namespace chroma::client::render::shader::shaders