#include "chroma/client/render/shader/shaders/ThresHoldPass.h"

namespace chroma::client::render::shader::shaders {
ThresHoldPass::ThresHoldPass(float threshold)
    : ShaderPass("resources/shaders/base.vs", "assets/shaders/threshold.fs"), threshold_(std::make_shared<float>(threshold))
{
    SetUniform("threshold", UniformType::FLOAT, threshold_);
}

void ThresHoldPass::Setup()
{
    LoadShader();

    for (auto &pair : values_) {
        const int loc = ::GetShaderLocation(shader_, pair.first.c_str());
        if (loc >= 0) {
        pair.second->SetLocation(loc);
        }
    }
}

void ThresHoldPass::Execute(RenderTexture2D &src, RenderTexture2D &dst)
{
    BeginTextureMode(dst);
    ClearBackground(BLANK);

    BeginShaderMode(shader_);
    UploadAll();
    DrawTextureRec(
        src.texture,
        Rectangle{0, 0, static_cast<float>(src.texture.width), static_cast<float>(-src.texture.height)},
        Vector2{0, 0},
        WHITE
    );
    EndShaderMode();
    EndTextureMode();
}

std::shared_ptr<float> ThresHoldPass::GetThreshold() const
{
    return threshold_;
}

void ThresHoldPass::SetThreshold(float threshold)
{
    *threshold_ = threshold;
}

} // namespace chroma::client::render::shader::shaders