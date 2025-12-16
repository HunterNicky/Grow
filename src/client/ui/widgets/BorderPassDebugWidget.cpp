#include "chroma/client/ui/widgets/BorderPassDebugWidget.h"
#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/client/render/RenderBridgeImpl.h"
#include "chroma/shared/render/RenderBridge.h"
#include "chroma/client/ui/UIContext.h"
#include <raygui.h>
#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {

BorderPassDebugWidget::BorderPassDebugWidget(const std::string &id, const Rectangle bounds)
  : Widget(id, bounds)
{
}

void BorderPassDebugWidget::SetBorderPass(render::shader::shaders::BorderPass* border_pass)
{
  border_pass_ = border_pass;
  if (border_pass_) {
    UpdateValues();
  }
}

void BorderPassDebugWidget::UpdateValues()
{
  if (!border_pass_) return;

  spread_value_ = border_pass_->GetSpread();
  
  if (auto* blur = border_pass_->GetBlurPass()) {
    blur_radius_ = blur->GetRadius();
  }
  
  if (auto* dilation = border_pass_->GetDilatationPass()) {
    dilation_radius_ = dilation->GetRadius();
  }
  
  if (auto* noise = border_pass_->GetNoisePass()) {
    noise_seed_ = noise->GetSeed();
  }

  hysteresis_iterations_ = border_pass_->GetHysteresisIterations();
  composite_with_original_ = border_pass_->IsCompositeWithOriginal();

  gray_enabled_ = border_pass_->IsGrayEnabled();
  angle_mag_enabled_ = border_pass_->IsAngleMagEnabled();
  non_maximum_enabled_ = border_pass_->IsNonMaximumEnabled();
  hysteresis_enabled_ = border_pass_->IsHysteresisEnabled();
  threshold_enabled_ = border_pass_->IsThresholdEnabled();
  blur_enabled_ = border_pass_->IsBlurEnabled();
  dilatation_enabled_ = border_pass_->IsDilatationEnabled();
  noise_enabled_ = border_pass_->IsNoiseEnabled();
}

void BorderPassDebugWidget::ApplyValues()
{
  if (!border_pass_) return;

  border_pass_->SetSpread(spread_value_);
  
  if (auto* blur = border_pass_->GetBlurPass()) {
    blur->SetRadius(blur_radius_);
  }
  
  if (auto* dilation = border_pass_->GetDilatationPass()) {
    dilation->SetRadius(dilation_radius_);
  }
  
  if (auto* noise = border_pass_->GetNoisePass()) {
    noise->SetSeed(noise_seed_);
  }

  border_pass_->SetHysteresisIterations(hysteresis_iterations_);
  border_pass_->SetCompositeWithOriginal(composite_with_original_);

  border_pass_->SetGrayEnabled(gray_enabled_);
  border_pass_->SetAngleMagEnabled(angle_mag_enabled_);
  border_pass_->SetNonMaximumEnabled(non_maximum_enabled_);
  border_pass_->SetHysteresisEnabled(hysteresis_enabled_);
  border_pass_->SetThresholdEnabled(threshold_enabled_);
  border_pass_->SetBlurEnabled(blur_enabled_);
  border_pass_->SetDilatationEnabled(dilatation_enabled_);
  border_pass_->SetNoiseEnabled(noise_enabled_);
}

void BorderPassDebugWidget::OnUpdate([[maybe_unused]] const float delta_time, 
                                      [[maybe_unused]] const UIContext &context)
{
}

void BorderPassDebugWidget::OnRender()
{
  if (!show_window_) return;

  if (!border_pass_) {
    auto bridge = shared::render::GetRenderBridge();
    if (bridge) {
      auto* impl = dynamic_cast<render::RenderBridgeImpl*>(bridge.get());
      if (impl && impl->GetRenderer() && impl->GetRenderer()->GetRenderPipeline()) {
        border_pass_ = impl->GetRenderer()->GetRenderPipeline()->GetPass<render::shader::shaders::BorderPass>();
        if (border_pass_) {
          UpdateValues();
        }
      }
    }
  }

  if (!border_pass_) {
    if (GuiWindowBox(bounds_, "Border Pass Debugger")) {
      show_window_ = false;
      return;
    }
    
    float cursor_y = bounds_.y + 30;
    constexpr float padding = 10;
    constexpr float line_height = 25;
    const float content_width = bounds_.width - (2 * padding);
    const float content_x = bounds_.x + padding;
    
    GuiLabel(Rectangle{ content_x, cursor_y, content_width, line_height }, "BorderPass not active in pipeline");
    return;
  }

  if (GuiWindowBox(bounds_, "Border Pass Debugger")) {
    show_window_ = false;
    return;
  }

  float cursor_y = bounds_.y + 30;
  constexpr float padding = 10;
  constexpr float line_height = 25;
  constexpr float checkbox_size = 20;
  const float content_width = bounds_.width - (2 * padding);
  const float content_x = bounds_.x + padding;

  GuiLabel(Rectangle{ content_x, cursor_y, content_width, line_height }, "=== Shader Parameters ===");
  cursor_y += line_height + 5;

  const std::string spread_label = TextFormat("Spread: %.4f", spread_value_);
  GuiLabel(Rectangle{ content_x, cursor_y, content_width * 0.3F, line_height }, spread_label.c_str());
  GuiSlider(Rectangle{ content_x + content_width * 0.35F, cursor_y, content_width * 0.6F, line_height },
            nullptr, nullptr, &spread_value_, 0.0F, 0.05F);
  ApplyValues();
  cursor_y += line_height + 5;

  const std::string blur_radius_label = TextFormat("Blur Radius: %d", blur_radius_);
  GuiLabel(Rectangle{ content_x, cursor_y, content_width * 0.3F, line_height }, blur_radius_label.c_str());
  float blur_radius_float = static_cast<float>(blur_radius_);
  GuiSlider(Rectangle{ content_x + content_width * 0.35F, cursor_y, content_width * 0.6F, line_height },
            nullptr, nullptr, &blur_radius_float, 1.0F, 30.0F);
  blur_radius_ = static_cast<int>(blur_radius_float);
  ApplyValues();
  cursor_y += line_height + 5;

  const std::string dilation_label = TextFormat("Dilation R: %d", dilation_radius_);
  GuiLabel(Rectangle{ content_x, cursor_y, content_width * 0.3F, line_height }, dilation_label.c_str());
  float dilation_float = static_cast<float>(dilation_radius_);
  GuiSlider(Rectangle{ content_x + content_width * 0.35F, cursor_y, content_width * 0.6F, line_height },
            nullptr, nullptr, &dilation_float, 1.0F, 10.0F);
  dilation_radius_ = static_cast<int>(dilation_float);
  ApplyValues();
  cursor_y += line_height + 5;

  const std::string noise_label = TextFormat("Noise Seed: %.2f", noise_seed_);
  GuiLabel(Rectangle{ content_x, cursor_y, content_width * 0.3F, line_height }, noise_label.c_str());
  GuiSlider(Rectangle{ content_x + content_width * 0.35F, cursor_y, content_width * 0.6F, line_height },
            nullptr, nullptr, &noise_seed_, 0.0F, 1000.0F);
  ApplyValues();
  cursor_y += line_height + 5;

  const std::string hysteresis_label = TextFormat("Hysteresis Iter: %d", hysteresis_iterations_);
  GuiLabel(Rectangle{ content_x, cursor_y, content_width * 0.3F, line_height }, hysteresis_label.c_str());
  float hysteresis_float = static_cast<float>(hysteresis_iterations_);
  GuiSlider(Rectangle{ content_x + content_width * 0.35F, cursor_y, content_width * 0.6F, line_height },
            nullptr, nullptr, &hysteresis_float, 1.0F, 20.0F);
  hysteresis_iterations_ = static_cast<int>(hysteresis_float);
  ApplyValues();
  cursor_y += line_height + 10;

  GuiCheckBox(Rectangle{ content_x, cursor_y, 20, 20 }, 
              "Composite with Original", &composite_with_original_);
  ApplyValues();
  cursor_y += line_height + 5;

  GuiLine(Rectangle{ content_x, cursor_y, content_width, 1 }, nullptr);
  cursor_y += 5;
  GuiLabel(Rectangle{ content_x, cursor_y, content_width, line_height }, "=== Pipeline Stages ===");
  cursor_y += line_height + 5;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Grayscale", &gray_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Angle Magnitude", &angle_mag_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Non-Maximum Suppression", &non_maximum_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Hysteresis", &hysteresis_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Threshold", &threshold_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Blur", &blur_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Dilatation", &dilatation_enabled_);
  cursor_y += line_height;

  GuiCheckBox(Rectangle{ content_x, cursor_y, checkbox_size, checkbox_size }, 
              "Noise", &noise_enabled_);
  ApplyValues();
  cursor_y += line_height + 10;

  if (GuiButton(Rectangle{ content_x, cursor_y, content_width, line_height }, "Reset to Defaults") == 1) {
    spread_value_ = 0.005F;
    blur_radius_ = 10;
    dilation_radius_ = 1;
    noise_seed_ = 0.0F;
    hysteresis_iterations_ = 5;
    composite_with_original_ = false;
    gray_enabled_ = true;
    angle_mag_enabled_ = true;
    non_maximum_enabled_ = true;
    hysteresis_enabled_ = true;
    threshold_enabled_ = true;
    blur_enabled_ = true;
    dilatation_enabled_ = true;
    noise_enabled_ = true;
    ApplyValues();
  }
}

}// namespace chroma::client::ui::widget
