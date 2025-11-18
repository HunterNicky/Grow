#include "chroma/client/render/shader/RenderPipeline.h"
#include <raylib.h>

namespace chroma::client::render::shader {
    
    RenderPipeline::~RenderPipeline() {
        UnloadRenderTexture(ping_);
        UnloadRenderTexture(pong_);
    }

    void RenderPipeline::Initialize(int width, int height) {
        ping_ = LoadRenderTexture(width, height);
        pong_ = LoadRenderTexture(width, height);
    }

    void RenderPipeline::AddPass(std::unique_ptr<RenderPass> pass) {
        passes_.push_back(std::move(pass));
    }

    void RenderPipeline::Setup() {
        for (auto& pass : passes_) {
            pass->Setup();
        }
    }

    RenderTexture2D& RenderPipeline::Execute(RenderTexture2D& original_scene) {
        
        RenderTexture2D* src = &original_scene;
        RenderTexture2D* dst = &ping_; 

        for (auto& pass : passes_) {
            pass->Execute(*src, *dst);
            src = dst; 
            if (dst == &ping_){
                dst = &pong_;
            } 
            else {
                dst = &ping_;
            } 
        }
        return *src;
    }
} // namespace chroma::client::render::shader