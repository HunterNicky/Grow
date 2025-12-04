#include "chroma/client/ui/widgets/ShaderDebugWidget.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ShaderEvent.h"
#include "chroma/shared/render/RenderBridge.h"
#include <raygui.h>
#include <string>

namespace chroma::client::ui::widget {

static std::string GetShaderName(uint8_t type)
{
  switch (static_cast<render::shader::PassType>(type)) {
  case render::shader::PassType::BLUR:
    return "BLUR";
  case render::shader::PassType::CRT:
    return "CRT";
  case render::shader::PassType::BORDER:
    return "BORDER";
  case render::shader::PassType::DERIVATIVEX:
    return "DX";
  case render::shader::PassType::DERIVATIVEY:
    return "DY";
  case render::shader::PassType::GRAYSCALE:
    return "GRAY";
  case render::shader::PassType::HEALTH:
    return "HEALTH";
  case render::shader::PassType::THRESHOLD:
    return "THRESH";
  case render::shader::PassType::BORDERCOLOR:
    return "BORDER_COLOR";
  default:
    return "Unknown";
  }
}

ShaderDebugWidget::ShaderDebugWidget(const std::string &id, const Rectangle bounds) : Widget(id, bounds)
{
  shader_names_ = { "BLUR", "CRT", "BORDER", "DX", "DY", "GRAY", "HEALTH", "THRESH", "BORDER_COLOR" };
  shader_types_ = { render::shader::PassType::BLUR,
    render::shader::PassType::CRT,
    render::shader::PassType::BORDER,
    render::shader::PassType::DERIVATIVEX,
    render::shader::PassType::DERIVATIVEY,
    render::shader::PassType::GRAYSCALE,
    render::shader::PassType::HEALTH,
    render::shader::PassType::THRESHOLD,
    render::shader::PassType::BORDERCOLOR };
}

void ShaderDebugWidget::OnUpdate([[maybe_unused]] const float delta_time, [[maybe_unused]] const UIContext &context) {}

void ShaderDebugWidget::OnRender()
{
  if (!show_window_) return;

  if (GuiWindowBox(bounds_, "Shader Debugger")) { show_window_ = false; }

  float cursor_y = bounds_.y + 30;
  constexpr float padding = 10;
  constexpr float line_height = 25;
  const float content_width = bounds_.width - (2 * padding);
  const float content_x = bounds_.x + padding;

  GuiLabel({ content_x, cursor_y, content_width, line_height }, "Add Shader:");
  cursor_y += line_height;

  const Rectangle dropdown_rect = { content_x, cursor_y, content_width * 0.7f, line_height };

  if (!dropdown_edit_mode_) {
    if (GuiButton({ content_x + content_width * 0.75f, cursor_y, content_width * 0.25f, line_height }, "Add")) {
      shared::event::ShaderEvent ev(shared::event::ShaderEventType::ADD);
      ev.SetPassType(shader_types_[selected_shader_index_]);
      ev.SetFront(false);
      shared::event::EventBus::Dispatch(ev);
    }
  }
  cursor_y += line_height + padding;

  GuiLine({ content_x, cursor_y, content_width, 1 }, "Active Shaders");
  cursor_y += padding;

  if (dropdown_edit_mode_) GuiLock();

  const auto active_passes = shared::render::GetRenderBridge()->GetActivePasses();

  for (size_t i = 0; i < active_passes.size(); i++) {
    uint8_t type_byte = active_passes[i];
    const auto pass_type = static_cast<render::shader::PassType>(type_byte);
    std::string name = GetShaderName(type_byte);

    const Rectangle row_rect = { content_x, cursor_y, content_width, line_height };

    GuiLabel({ row_rect.x, row_rect.y, content_width * 0.3f, line_height }, name.c_str());

    float btn_x = row_rect.x + content_width * 0.3f;

    if (GuiButton({ btn_x, row_rect.y, 40, line_height }, "Rld")) {
      shared::event::ShaderEvent ev(shared::event::ShaderEventType::RELOAD);
      ev.SetPassType(pass_type);
      shared::event::EventBus::Dispatch(ev);
    }
    btn_x += 45;

    if (i > 0) {
      if (GuiButton({ btn_x, row_rect.y, 25, line_height }, "^")) {
        shared::event::ShaderEvent ev(shared::event::ShaderEventType::MOVE_UP);
        ev.SetPassType(pass_type);
        shared::event::EventBus::Dispatch(ev);
      }
    }
    btn_x += 30;

    if (i < active_passes.size() - 1) {
      if (GuiButton({ btn_x, row_rect.y, 25, line_height }, "v")) {
        shared::event::ShaderEvent ev(shared::event::ShaderEventType::MOVE_DOWN);
        ev.SetPassType(pass_type);
        shared::event::EventBus::Dispatch(ev);
      }
    }
    btn_x += 30;

    if (GuiButton({ btn_x, row_rect.y, 25, line_height }, "X")) {
      shared::event::ShaderEvent ev(shared::event::ShaderEventType::REMOVE);
      ev.SetPassType(pass_type);
      shared::event::EventBus::Dispatch(ev);
    }

    cursor_y += line_height + 2;
  }

  GuiUnlock();

  if (GuiDropdownBox(dropdown_rect,
        "BLUR;CRT;BORDER;DX;DY;GRAY;HEALTH;THRESH;BORDER_COLOR",
        &selected_shader_index_,
        dropdown_edit_mode_)) {
    dropdown_edit_mode_ = !dropdown_edit_mode_;
  }
}

}// namespace chroma::client::ui::widget