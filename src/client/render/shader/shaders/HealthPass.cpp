#include "chroma/client/render/shader/shaders/HealthPass.h"
#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/components/Health.h"

#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

namespace chroma::client::render::shader::shaders {

HealthPass::HealthPass() : ShaderPass("resources/shaders/base.vs", "assets/shaders/health.fs")
{
  const auto player = GCM::Instance().GetContext(GameContextType::Client)->GetLocalPlayer();
  if (!player) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_WARNING, "HealthPass: No local player found in GameContext.");
    return;
  }

  const auto health = player->GetComponent<shared::core::component::Health>();
  if (!health) {
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_WARNING, "HealthPass: Local player has no Health component.");
    return;
  }

  SetPassType(PassType::HEALTH);
  SetUniform("health", UniformType::FLOAT, health->GetCurrentHealth());
  SetUniform("max_health", UniformType::FLOAT, health->GetMaxHealth());
  SetUniform("time", UniformType::FLOAT, GCM::Instance().GetContext(GameContextType::Client)->GetDeltaTime());
}

void HealthPass::Setup()
{
  LoadShader();

  for (auto &pair : values_) {
    const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
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