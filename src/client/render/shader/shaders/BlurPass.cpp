#include "chroma/client/render/shader/shaders/BlurPass.h"
#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <algorithm>
#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

BlurPass::BlurPass(const int width, const int height, int initial_radius)
  : ShaderPass("resources/shaders/base.vs", "assets/shaders/blur.fs"),
    resolution_(std::make_shared<Vector2>(Vector2{ static_cast<float>(width), static_cast<float>(height) }))
{
  SetPassType(PassType::BLUR);
  SetUniform("u_resolution", UniformType::VEC2, resolution_);

  radius_ = std::make_shared<int>(initial_radius);
  SetUniform("u_radius", UniformType::INT, radius_);
}

void BlurPass::SetRadius(int new_radius)
{
  new_radius = std::max(new_radius, 0);
  *radius_ = new_radius;
  SetUniform("u_radius", UniformType::INT, radius_);
}

int BlurPass::GetRadius() const { return *radius_; }

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

void BlurPass::SetResolution(const int width, const int height)
{
  resolution_->x = static_cast<float>(width);
  resolution_->y = static_cast<float>(height);
  SetUniform("u_resolution", UniformType::VEC2, resolution_);
}

}// namespace chroma::client::render::shader::shaders