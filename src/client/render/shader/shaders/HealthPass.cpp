#include "chroma/client/render/shader/shaders/HealthPass.h"

#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/core/components/Health.h"

#include <memory>
#include <string>
#include <utility>

namespace chroma::client::render::shader::shaders {

HealthPass::HealthPass() : ShaderPass("resources/shaders/base.vs", "assets/shaders/health.fs")
{
  auto player = shared::context::GameContext::GetInstance().GetLocalPlayer();
  if (!player) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_WARNING, "HealthPass: No local player found in GameContext.");
    return;
  }

  auto health = player->GetComponent<shared::core::component::Health>();
  if (!health) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_WARNING, "HealthPass: Local player has no Health component.");
    return;
  }
  SetUniform("health", UniformType::FLOAT, health->GetCurrentHealth());
  SetUniform("max_health", UniformType::FLOAT, health->GetMaxHealth());
  SetUniform("time", UniformType::FLOAT, shared::context::GameContext::GetInstance().GetDeltaTime());
}

void HealthPass::Setup()
{
  LoadShader();

  for (auto &pair : values_) {
    int loc = ::GetShaderLocation(shader_, pair.first.c_str());
    if (loc >= 0) { pair.second->SetLocation(loc); }
  }
}

void HealthPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
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