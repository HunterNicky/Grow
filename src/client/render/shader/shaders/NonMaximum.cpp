#include "chroma/client/render/shader/shaders/NonMaximum.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {
NonMaximum::NonMaximum() : ShaderPass("assets/shaders/base.vs", "assets/shaders/non_maximum.fs")
{
  SetPassType(PassType::NON_MAXIMUM);
}

void NonMaximum::Setup()
{
  LoadShader();
  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void NonMaximum::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
  BeginTextureMode(dst);
  ClearBackground(BLANK);

  BeginShaderMode(shader_);
  UploadAll();
  DrawTextureRec(src.texture,
    Rectangle{ 0, 0, static_cast<float>(dst.texture.width), static_cast<float>(-src.texture.height) },
    Vector2{ 0, 0 },
    WHITE);
  EndShaderMode();
  EndTextureMode();
}
}// namespace chroma::client::render::shader::shaders