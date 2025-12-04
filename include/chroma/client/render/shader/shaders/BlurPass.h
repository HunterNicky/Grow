#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class BlurPass : public ShaderPass
{
public:
  explicit BlurPass(int width = 1920, int height = 1080, Vector2 initial_direction = { 1.0f, 0.0f });

  void SetDirection(const Vector2 &direction);
  int GetRadius() const;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

private:
  std::shared_ptr<Vector2> resolution_;
  std::shared_ptr<Vector2> direction_;
};
}// namespace chroma::client::render::shader::shaders