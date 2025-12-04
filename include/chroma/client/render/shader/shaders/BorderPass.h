#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/AngleMagPass.h"
#include "chroma/client/render/shader/shaders/BlurPass.h"
#include "chroma/client/render/shader/shaders/GrayPass.h"
#include "chroma/client/render/shader/shaders/Hysteresis.h"
#include "chroma/client/render/shader/shaders/NonMaximum.h"
#include "chroma/client/render/shader/shaders/ThresholdPass.h"
#include "chroma/client/render/shader/shaders/DilatationPass.h"
#include "chroma/client/render/shader/shaders/NoisePass.h"

#include <memory>
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class BorderPass : public ShaderPass
{
public:
  explicit BorderPass(int width = 1920, int height = 1080);
  ~BorderPass() override;

  BorderPass(const BorderPass &) = delete;
  BorderPass &operator=(const BorderPass &) = delete;
  BorderPass(BorderPass &&) noexcept = default;
  BorderPass &operator=(BorderPass &&) noexcept = default;

  void Setup() override;
  void Execute(RenderTexture2D &src, RenderTexture2D &dst) override;

private:
  std::unique_ptr<AngleMagPass> pass_angle_mag__;
  std::unique_ptr<GrayPass> pass_gray_;
  std::unique_ptr<NonMaximum> pass_non_maximum_;
  std::unique_ptr<Hysteresis> pass_hysteresis_;
  std::unique_ptr<ThresholdPass> pass_threshold_;
  std::unique_ptr<BlurPass> pass_blur_;
  std::unique_ptr<DilatationPass> pass_dilatation_;
  std::unique_ptr<NoisePass> pass_noise_;

  std::shared_ptr<RenderTexture2D> tex_angle_mag_;
  std::shared_ptr<RenderTexture2D> tex_non_maximum_;
  std::shared_ptr<RenderTexture2D> tex_dilated_;
  std::shared_ptr<RenderTexture2D> tex_noise_;

  std::shared_ptr<RenderTexture2D> ping_;
  std::shared_ptr<RenderTexture2D> pong_;

  std::shared_ptr<int> slot_edges_;
  std::shared_ptr<int> slot_noise_val_;
  std::shared_ptr<float> spread_val_;
  std::shared_ptr<Vector2> resolution_;

};

}// namespace chroma::client::render::shader::shaders
