#pragma once

#include "RenderPass.h"
#include <vector>
#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader {

class RenderPipeline {
public:
    RenderPipeline() = default;
    ~RenderPipeline();
    RenderPipeline(const RenderPipeline&) = delete;
    RenderPipeline& operator=(const RenderPipeline&) = delete;
    RenderPipeline(RenderPipeline&&) noexcept = default;
    RenderPipeline& operator=(RenderPipeline&&) noexcept = default;
    
    void Initialize(int width, int height);

    void AddPassFront(std::unique_ptr<RenderPass> pass);
    void AddPassBack(std::unique_ptr<RenderPass> pass);

    void Setup();

    RenderTexture2D& Execute(RenderTexture2D& original_scene);

private:
    std::vector<std::unique_ptr<RenderPass>> passes_;
    
    RenderTexture2D ping_{};
    RenderTexture2D pong_{};
};

}