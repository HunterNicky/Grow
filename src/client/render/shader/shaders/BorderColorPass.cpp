#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/BorderColorPass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

BorderColorPass::BorderColorPass() : ShaderPass("assets/shaders/base.vs", "assets/shaders/border_color.fs")
{
  SetPassType(PassType::BORDERCOLOR);
}

void BorderColorPass::Setup()
{
  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void BorderColorPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
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
