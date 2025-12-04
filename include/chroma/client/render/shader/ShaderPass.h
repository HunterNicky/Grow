#pragma once

#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderValue.h"
#include <memory>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <utility>

namespace chroma::client::render::shader {

inline RenderTexture2D LoadFloatRenderTexture(const int width, const int height)
{
  RenderTexture2D target = { 0 };

  target.id = rlLoadFramebuffer();
  rlEnableFramebuffer(target.id);
  if (target.id > 0) {
    target.texture.id = rlLoadTexture(nullptr, width, height, PIXELFORMAT_UNCOMPRESSED_R32G32B32A32, 1);
    target.texture.width = width;
    target.texture.height = height;
    target.texture.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
    target.texture.mipmaps = 1;

    rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);

    if (rlFramebufferComplete(target.id)) {
      TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);
    } else {
      TraceLog(LOG_WARNING, "FBO: [ID %i] Framebuffer object not complete", target.id);
    }
  } else {
    TraceLog(LOG_WARNING, "FBO: Framebuffer object can not be created");
  }

  return target;
}

class ShaderPass : public RenderPass
{
public:
  ~ShaderPass() override { Unload(); }

  ShaderPass(const ShaderPass &) = delete;
  ShaderPass &operator=(const ShaderPass &) = delete;
  ShaderPass(ShaderPass &&other) = delete;
  ShaderPass &operator=(ShaderPass &&other) = delete;

  ShaderPass(std::string vs_path, std::string fs_path) : RenderPass(std::move(vs_path), std::move(fs_path)) {}

  template<typename T> void SetUniform(const std::string &name, rlShaderUniformDataType type, std::shared_ptr<T> value)
  {
    values_[name] = std::make_unique<ShaderValue<T>>(-1, type, value);
  }

  void UploadAll() const
  {
    for (const auto &pair : values_) { pair.second->SetValue(shader_); }
  }

  void LoadShader()
  {
    shader_ = ::LoadShader(vs_path_.c_str(), fs_path_.c_str());
    if (shader_.id == 0) {
      // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
      TraceLog(LOG_ERROR, "Failed to load shader: %s / %s", vs_path_.c_str(), fs_path_.c_str());
    }
  }

  void Unload()
  {
    if (shader_.id != 0) {
      ::UnloadShader(shader_);
      shader_.id = 0;
    }
  }

  void Reload() override
  {
    Unload();
    LoadShader();

    for (auto &pair : values_) {
      const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
      if (loc >= 0) { pair.second->SetLocation(loc); }
    }

    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
    TraceLog(LOG_INFO, "ShaderPass: Reloaded shader %s", fs_path_.c_str());
  }

protected:
  ::Shader shader_{};
  std::unordered_map<std::string, std::unique_ptr<IShaderValue>> values_;
};
}// namespace chroma::client::render::shader