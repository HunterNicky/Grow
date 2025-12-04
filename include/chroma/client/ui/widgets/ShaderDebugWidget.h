#pragma once

#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/ui/widgets/Widget.h"
#include <string>
#include <vector>

namespace chroma::client::ui::widget {

class ShaderDebugWidget : public Widget
{
public:
  ShaderDebugWidget(const std::string &id, Rectangle bounds);

  void OnUpdate(const float delta_time, const UIContext &context) override;
  void OnRender() override;

private:
  bool show_window_ = true;
  int selected_shader_index_ = 0;
  bool dropdown_edit_mode_ = false;

  std::vector<std::string> shader_names_;
  std::vector<render::shader::PassType> shader_types_;
};

}// namespace chroma::client::ui::widget