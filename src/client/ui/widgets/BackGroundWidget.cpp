#include "chroma/client/ui/widgets/BackGroundWidget.h"
#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {
BackGroundWidget::BackGroundWidget(const std::string &id, const Rectangle bounds, const Color color)
  : Widget(id, bounds), background_color_(color)
{}

void BackGroundWidget::OnUpdate([[maybe_unused]] const float delta_time, [[maybe_unused]] const UIContext &context) {}

void BackGroundWidget::OnRender() { DrawRectangleRec(bounds_, background_color_); }
}// namespace chroma::client::ui::widget