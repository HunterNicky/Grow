#include "chroma/client/render/shader/RenderShader.h"

#include <raylib.h>
#include <algorithm>
#include <string>

namespace chroma::client::render::shader {

RenderShader& RenderShader::GetInstance()
{
    static RenderShader render_shader;
    return render_shader;
}

void RenderShader::AddShader(ShaderData&& data, std::string& name)
{
    if (data.GetShader().id == 0) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-type-vararg)
        TraceLog(LOG_WARNING, "Shader '%s' failed to load. Ignoring.", name.c_str());
        return;
    }
    shaders_[name] = std::move(data);
}


void RenderShader::RemoveShader(const std::string& name)
{
    shaders_.erase(name);
}

void RenderShader::BeginRenderUniqueShader(const std::string& name)
{
    auto it = shaders_.find(name);
    if (it != shaders_.end()) {
        BeginShaderMode(it->second.GetShader());
        it->second.UploadAll();
    }
}

void RenderShader::EndRenderUniqueShader() 
{
    EndShaderMode();
}

bool RenderShader::HasShaders() const
{
    return !shaders_.empty();
}

std::vector<ShaderData*> RenderShader::GetSortedShaders()
{
    std::vector<ShaderData*> sorted_shaders;
    
    sorted_shaders.reserve(shaders_.size());
    for (auto& [name, shader_data] : shaders_) {
        sorted_shaders.push_back(&shader_data);
    }

    std::ranges::sort(sorted_shaders,
    [](ShaderData* a, ShaderData* b) {
        return a->GetPriority() < b->GetPriority();
    });
    return sorted_shaders;
}
} // namespace chroma::client::render::shader