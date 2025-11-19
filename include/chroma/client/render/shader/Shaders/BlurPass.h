#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <raylib.h>
#include <memory>

namespace chroma::client::render::shader::shaders {

class BlurPass : public ShaderPass {
public:
    BlurPass(int width, int height, int initial_radius = 6);

    void SetRadius(int new_radius);

    int GetRadius() const;
    
    void Setup() override;
    void Execute(RenderTexture2D& src, RenderTexture2D& dst) override;

private:
    std::shared_ptr<Vector2> resolution_;
    std::shared_ptr<int> radius_;
};
} // chroma::client::render::shader::shaders