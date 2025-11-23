#include "chroma/app/states/mediator/RenderMediator.h"
#include "chroma/client/render/Renderer.h"
#include <memory>
#include <utility>

namespace chroma::app::states::mediator {
RenderMediator::RenderMediator(std::shared_ptr<client::render::Renderer> render) : renderer_(std::move(render)) {}

void RenderMediator::AddShaderFront(std::unique_ptr<chroma::client::render::shader::RenderPass> pass)
{
  renderer_->AddShaderPassFront(std::move(pass));
}

void RenderMediator::AddShaderBack(std::unique_ptr<chroma::client::render::shader::RenderPass> pass)
{
  renderer_->AddShaderPassBack(std::move(pass));
}
}// namespace chroma::app::states::mediator