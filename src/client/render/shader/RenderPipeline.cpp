#include "chroma/client/render/shader/RenderPipeline.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/ShaderPass.h"

#include <memory>
#include <raylib.h>
#include <utility>

namespace chroma::client::render::shader {

RenderPipeline::~RenderPipeline()
{
  UnloadRenderTexture(ping_);
  UnloadRenderTexture(pong_);
}

void RenderPipeline::Initialize(const int width, const int height)
{
  ping_ = LoadRenderTexture(width, height);
  pong_ = LoadRenderTexture(width, height);
}

void RenderPipeline::AddPassFront(std::unique_ptr<RenderPass> pass)
{
  passes_.insert(passes_.begin(), std::move(pass));
}

void RenderPipeline::AddPassBack(std::unique_ptr<RenderPass> pass) { passes_.push_back(std::move(pass)); }

void RenderPipeline::Setup() const
{
  for (const auto &pass : passes_) { pass->Setup(); }
}

RenderTexture2D &RenderPipeline::Execute(RenderTexture2D &original_scene)
{

  RenderTexture2D *src = &original_scene;
  RenderTexture2D *dst = &ping_;

  for (const auto &pass : passes_) {
    if (!pass->IsActive()) { continue; }
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

RenderPass *RenderPipeline::GetPassByType(const PassType type) const
{
  for (const auto &p : passes_) {
    if (const auto &shader_pass = dynamic_cast<ShaderPass *>(p.get())) {
      if (shader_pass->GetPassType() == type) { return shader_pass; }
    }
  }
  return nullptr;
}

void RenderPipeline::RemovePassByType(PassType type)
{
  std::erase_if(passes_, [type](const std::unique_ptr<RenderPass> &p) {
    if (const auto *shader_pass = dynamic_cast<ShaderPass *>(p.get())) { return shader_pass->GetPassType() == type; }
    return false;
  });
}

void RenderPipeline::ReloadPassByType(const PassType type) const
{
  RenderPass *pass = GetPassByType(type);
  if (pass != nullptr) { pass->Reload(); }
}
void RenderPipeline::MovePass(PassType type, int direction)
{
  const auto it = std::ranges::find_if(passes_, [type](const std::unique_ptr<RenderPass> &p) {
    const auto *sp = dynamic_cast<ShaderPass *>(p.get());
    return sp && sp->GetPassType() == type;
  });

  if (it == passes_.end()) return;

  const size_t index = std::distance(passes_.begin(), it);

  if (direction < 0 && index > 0) {
    std::swap(passes_[index], passes_[index - 1]);
  } else if (direction > 0 && index < passes_.size() - 1) {
    std::swap(passes_[index], passes_[index + 1]);
  }
}

std::vector<PassType> RenderPipeline::GetActivePassTypes() const
{
  std::vector<PassType> types;
  for (const auto &pass : passes_) {
    if (pass) types.push_back(pass->GetPassType());
  }
  return types;
}

}// namespace chroma::client::render::shader