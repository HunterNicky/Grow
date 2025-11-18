#include "chroma/client/render/shader/shaders/CrtPass.h"

namespace chroma::client::render::shader::shaders {

CrtPass::CrtPass() 
    : ShaderPass("resources/shaders/base.vs", "assets/shaders/crt.fs") 
{
}

void CrtPass::Setup() {
        LoadShader();
        
        for (auto& pair : values_) {
            int loc = ::GetShaderLocation(shader_, pair.first.c_str());
            if (loc >= 0) {
                pair.second->SetLocation(loc);
            }
        }
    }

void CrtPass::Execute(RenderTexture2D& src, RenderTexture2D& dst) {
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