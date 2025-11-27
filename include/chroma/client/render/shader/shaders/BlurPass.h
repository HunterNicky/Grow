#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class BlurPass : public ShaderPass
{
public:
  explicit BlurPass(int width = 1920, int height = 1080, int initial_radius = 6);

  void SetRadius(int new_radius);
  void SetResolution(int width, int height);
  int GetRadius() const;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

private:
  std::shared_ptr<Vector2> resolution_;
  std::shared_ptr<int> radius_;
};
}// namespace chroma::client::render::shader::shaders