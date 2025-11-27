#include "chroma/client/render/shader/RenderPipeline.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <algorithm>
#include <memory>
#include <raylib.h>
#include <utility>

namespace chroma::client::render::shader {

RenderPipeline::~RenderPipeline()
{
  UnloadRenderTexture(ping_);
  UnloadRenderTexture(pong_);
}

void RenderPipeline::Initialize(int width, int height)
{
  ping_ = LoadRenderTexture(width, height);
  pong_ = LoadRenderTexture(width, height);
}

void RenderPipeline::AddPassFront(std::unique_ptr<RenderPass> pass)
{
  passes_.insert(passes_.begin(), std::move(pass));
}

void RenderPipeline::AddPassBack(std::unique_ptr<RenderPass> pass) { passes_.push_back(std::move(pass)); }

void RenderPipeline::Setup()
{
  for (auto &pass : passes_) { pass->Setup(); }
}

RenderTexture2D &RenderPipeline::Execute(RenderTexture2D &original_scene)
{

  RenderTexture2D *src = &original_scene;
  RenderTexture2D *dst = &ping_;

  for (auto &pass : passes_) {
    if(!pass->IsActive()) { continue; }
    pass->Execute(*src, *dst);
    src = dst;
    if (dst == &ping_) {
      dst = &pong_;
    } else {
      dst = &ping_;
    }
  }
  return *src;
}

RenderPass* RenderPipeline::GetPassByType(PassType type) const
{
  for (const auto &p : passes_) {
    if (const auto &shader_pass = dynamic_cast<ShaderPass*>(p.get()))
    {
      if (shader_pass->GetPassType() == type) { return shader_pass; }
    }
  }
  return nullptr;
}

void RenderPipeline::RemovePassByType(PassType type)
{
  std::erase_if(passes_, [type](const std::unique_ptr<RenderPass>& p) {
    if (auto* shader_pass = dynamic_cast<ShaderPass*>(p.get())) {
      return shader_pass->GetPassType() == type;
    }
    return false;
  });
}

}// namespace chroma::client::render::shader