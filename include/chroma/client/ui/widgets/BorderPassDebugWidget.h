#pragma once

#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {

class BorderPassDebugWidget : public Widget
{
public:
  explicit BorderPassDebugWidget(const std::string &id, Rectangle bounds);

  void OnUpdate(float delta_time, const UIContext &context) override;
  void OnRender() override;

  void SetBorderPass(render::shader::shaders::BorderPass* border_pass);
  void Show() { show_window_ = true; }
  void Hide() { show_window_ = false; }
  void Toggle() { show_window_ = !show_window_; }
  bool IsVisible() const { return show_window_; }

private:
  bool show_window_ = true;
  render::shader::shaders::BorderPass* border_pass_ = nullptr;

  float spread_value_ = 0.005F;
  int blur_radius_ = 10;
  int dilation_radius_ = 1;
  float noise_seed_ = 0.0F;
  int hysteresis_iterations_ = 5;
  bool composite_with_original_ = false;

  bool gray_enabled_ = true;
  bool angle_mag_enabled_ = true;
  bool non_maximum_enabled_ = true;
  bool hysteresis_enabled_ = true;
  bool threshold_enabled_ = true;
  bool blur_enabled_ = true;
  bool dilatation_enabled_ = true;
  bool noise_enabled_ = true;

  void UpdateValues();
  void ApplyValues();
};

}// namespace chroma::client::ui::widget
