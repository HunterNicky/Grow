#pragma once 

#include "chroma/client/render/Renderer.h"
#include "chroma/client/render/shader/RenderPass.h"

#include <memory>

namespace chroma::app::states::mediator {

class RenderMediator 
{
public:
    explicit RenderMediator(std::shared_ptr<client::render::Renderer> render);
    ~RenderMediator() = default;

    RenderMediator(const RenderMediator &) = default;
    RenderMediator &operator=(const RenderMediator &) = default;
    RenderMediator(RenderMediator &&) noexcept = default;
    RenderMediator &operator=(RenderMediator &&) noexcept = default;

    void AddShaderFront(std::unique_ptr<chroma::client::render::shader::RenderPass> pass);
    void AddShaderBack(std::unique_ptr<chroma::client::render::shader::RenderPass> pass);

private:
    std::shared_ptr<client::render::Renderer> renderer_;
};
} //chroma::app::states::mediator