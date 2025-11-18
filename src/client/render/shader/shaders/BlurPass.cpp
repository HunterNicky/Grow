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
} // namespace chroma::client::render::shader::shaders