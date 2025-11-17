#pragma once 

#include "chroma/client/render/shader/ShaderPass.h"
#include <raylib.h>

namespace chroma::client::render::shader {

class RenderShader {

public:
    ~RenderShader() = default;
    
    RenderShader(const RenderShader&) = delete;
    RenderShader& operator=(const RenderShader&) = delete;
    RenderShader(RenderShader&&) noexcept = default;
    RenderShader& operator=(RenderShader&&) noexcept = default;
    
    void AddShader(ShaderPass&& data, std::string& name);
    void RemoveShader(const std::string& name);
    
    void Initialize(int width, int height);
    static RenderShader& GetInstance();

    void SetupShaders();
    RenderTexture2D ApplyShaders(RenderTexture2D& target_texture);

private:
    RenderShader() = default;
    std::unordered_map<std::string, ShaderPass> shaders_;
    RenderTexture2D intermediate_texture_ {};
};

}// chroma::client::render::shader