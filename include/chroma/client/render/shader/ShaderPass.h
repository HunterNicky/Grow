#pragma once

#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/ShaderValue.h"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>

namespace chroma::client::render::shader {

class ShaderPass : public RenderPass { 
public:

    ~ShaderPass() override { Unload(); } 

    ShaderPass(const ShaderPass&) = delete;
    ShaderPass& operator=(const ShaderPass&) = delete;
    ShaderPass(ShaderPass&& other) = delete;
    ShaderPass& operator=(ShaderPass&& other) = delete;

    ShaderPass(std::string vs_path, std::string fs_path)
    : RenderPass(std::move(vs_path), std::move(fs_path)) {}
    
    template<typename T>
    void SetUniform(const std::string& name, UniformType type, std::shared_ptr<T> value) {
        values_[name] = std::make_unique<ShaderValue<T>>(-1, type, value);
    }

    void UploadAll() const {
        for (const auto& pair : values_) {
            pair.second->SetValue(shader_);
        }
    }

     void LoadShader() {
        shader_ = ::LoadShader(vs_path_.c_str(), fs_path_.c_str());
        if (shader_.id == 0) 
        {
            // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
            TraceLog(LOG_ERROR, "Failed to load shader: %s / %s", vs_path_.c_str(), fs_path_.c_str());
        }
    }

    void Unload() {
        if (shader_.id != 0) {
            ::UnloadShader(shader_);
            shader_.id = 0;
        }
    }

protected:
    ::Shader shader_{};
    std::unordered_map<std::string, std::unique_ptr<IShaderValue>> values_;
};
} // namespace chroma::client::render::shader