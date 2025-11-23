#pragma once
#include <raylib.h>
#include <string>

namespace chroma::client::render::shader {

class RenderPass
{
public:
  virtual ~RenderPass() = default;

  RenderPass(std::string vs_path, std::string fs_path) : vs_path_(std::move(vs_path)), fs_path_(std::move(fs_path)) {}
  RenderPass(const RenderPass &) = delete;
  RenderPass &operator=(const RenderPass &) = delete;
  RenderPass(RenderPass &&) = delete;
  RenderPass &operator=(RenderPass &&) = delete;

  virtual void Setup() = 0;
  virtual void Execute(RenderTexture2D &src, RenderTexture2D &dst) = 0;

protected:
  std::string vs_path_;
  std::string fs_path_;
};

}// namespace chroma::client::render::shader