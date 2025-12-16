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

  BlurPass* GetBlurPass() const { return pass_blur_.get(); }
  DilatationPass* GetDilatationPass() const { return pass_dilatation_.get(); }
  NoisePass* GetNoisePass() const { return pass_noise_.get(); }

  void SetSpread(float spread) { *spread_val_ = spread; }
  float GetSpread() const { return *spread_val_; }

  void SetGrayEnabled(bool enabled) { gray_enabled_ = enabled; }
  bool IsGrayEnabled() const { return gray_enabled_; }

  void SetAngleMagEnabled(bool enabled) { angle_mag_enabled_ = enabled; }
  bool IsAngleMagEnabled() const { return angle_mag_enabled_; }

  void SetNonMaximumEnabled(bool enabled) { non_maximum_enabled_ = enabled; }
  bool IsNonMaximumEnabled() const { return non_maximum_enabled_; }

  void SetHysteresisEnabled(bool enabled) { hysteresis_enabled_ = enabled; }
  bool IsHysteresisEnabled() const { return hysteresis_enabled_; }

  void SetThresholdEnabled(bool enabled) { threshold_enabled_ = enabled; }
  bool IsThresholdEnabled() const { return threshold_enabled_; }

  void SetBlurEnabled(bool enabled) { blur_enabled_ = enabled; }
  bool IsBlurEnabled() const { return blur_enabled_; }

  void SetDilatationEnabled(bool enabled) { dilatation_enabled_ = enabled; }
  bool IsDilatationEnabled() const { return dilatation_enabled_; }

  void SetNoiseEnabled(bool enabled) { noise_enabled_ = enabled; }
  bool IsNoiseEnabled() const { return noise_enabled_; }

  void SetHysteresisIterations(int iterations) { hysteresis_iterations_ = iterations; }
  int GetHysteresisIterations() const { return hysteresis_iterations_; }

  void SetCompositeWithOriginal(bool enabled) { composite_with_original_ = enabled; }
  bool IsCompositeWithOriginal() const { return composite_with_original_; }

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

  std::shared_ptr<RenderTexture2D> tex_composite_;
  Shader composite_shader_;

  std::shared_ptr<int> slot_edges_;
  std::shared_ptr<int> slot_noise_val_;
  std::shared_ptr<float> spread_val_;
  std::shared_ptr<Vector2> resolution_;

  bool gray_enabled_ = true;
  bool angle_mag_enabled_ = true;
  bool non_maximum_enabled_ = true;
  bool hysteresis_enabled_ = true;
  bool threshold_enabled_ = true;
  bool blur_enabled_ = true;
  bool dilatation_enabled_ = true;
  bool noise_enabled_ = true;
  
  int hysteresis_iterations_ = 5;
  bool composite_with_original_ = false;
};

}// namespace chroma::client::render::shader::shaders
