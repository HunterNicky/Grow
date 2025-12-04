#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include "chroma/client/render/shader/shaders/AngleMagPass.h"
#include "chroma/client/render/shader/shaders/GrayPass.h"
#include "chroma/client/render/shader/shaders/Hysteresis.h"
#include "chroma/client/render/shader/shaders/NonMaximum.h"
#include "chroma/client/render/shader/shaders/ThresholdPass.h"

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

  std::shared_ptr<RenderTexture2D> tex_angle_mag_;
  std::shared_ptr<RenderTexture2D> tex_non_maximum_;

  std::shared_ptr<RenderTexture2D> tex_hysteresis_ping;
  std::shared_ptr<RenderTexture2D> tex_hysteresis_pong;

  std::shared_ptr<int> slot_angle_mag_val_;
  std::shared_ptr<int> slot_non_maximum_val_;
};

}// namespace chroma::client::render::shader::shaders
