#include "chroma/client/render/shader/RenderShader.h"

#include <raylib.h>
#include <string>

namespace chroma::client::render::shader {

RenderShader& RenderShader::GetInstance()
{
    static RenderShader render_shader;
    return render_shader;
}

void RenderShader::AddShader(ShaderData&& data, std::string& name)
{
    shaders_[name] = std::move(data);
}


void RenderShader::RemoveShader(const std::string& name)
{
    shaders_.erase(name);
}

RenderTexture2D RenderShader::ApplyShaders(RenderTexture2D& target_texture)
{
    Texture2D source_texture =  target_texture.texture;

    for (auto& pair : shaders_)
    {
        shader::ShaderData* shader_data = &pair.second;

        BeginTextureMode(intermediate_texture_);
        ClearBackground(BLANK);

        BeginShaderMode(shader_data->GetShader());
        shader_data->UploadAll();
        DrawTexture(source_texture, 0, 0, WHITE);
        EndShaderMode();

        EndTextureMode();

        source_texture = intermediate_texture_.texture;
    }
    
    BeginTextureMode(target_texture);
    ClearBackground(BLANK);
    DrawTexture(source_texture, 0, 0, WHITE);
    EndTextureMode();

    return target_texture;
}

void RenderShader::SetupShaders()
{
    for (auto& pair : shaders_)
    {
        pair.second.Setup();
    }
}

void RenderShader::Initialize(int width, int height)
{
    intermediate_texture_ = LoadRenderTexture(width, height);
}

} // namespace chroma::client::render::shader