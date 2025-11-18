#include "chroma/client/render/shader/shaders/BlurPass.h"


namespace chroma::client::render::shader::shaders {

BlurPass::BlurPass(int width, int height, int initial_radius) 
    : ShaderPass("resources/shaders/base.vs", "assets/shaders/blur.fs") 
{
    resolution_ = std::make_shared<Vector2>((Vector2){(float)width, (float)height});
    SetUniform("u_resolution", UniformType::VEC2, resolution_);

    radius_ = std::make_shared<int>(initial_radius);
    SetUniform("u_radius", UniformType::INT, radius_);
}

void BlurPass::SetRadius(int new_radius) {
    std::max(new_radius, 0); 
    *radius_ = new_radius;
    SetUniform("u_radius", UniformType::INT, radius_);
}

int BlurPass::GetRadius() const {
    return *radius_;
}

void BlurPass::Setup() {
    LoadShader();
    
    for (auto& pair : values_) {
        int loc = ::GetShaderLocation(shader_, pair.first.c_str());
        if (loc >= 0) {
            pair.second->SetLocation(loc);
        }
    }
}

void BlurPass::Execute(RenderTexture2D& src, RenderTexture2D& dst) {
    BeginTextureMode(dst);
        ClearBackground(BLANK); 
        
        BeginShaderMode(shader_);
            UploadAll(); 
            DrawTextureRec(src.texture, 
                            (Rectangle){0, 0, (float)src.texture.width, (float)-src.texture.height}, 
                            (Vector2){0, 0}, WHITE);
        EndShaderMode();
    EndTextureMode();
}
} // namespace chroma::client::render::shader::shaders