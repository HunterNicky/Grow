#include "chroma/client/ui/widgets/BackGroundWidget.h"

namespace chroma::client::ui::widget {
BackGroundWidget::BackGroundWidget(const std::string& id, Rectangle bounds, Color color)
    : Widget(id, bounds), background_color_(color)
{
}

void BackGroundWidget::OnUpdate(const float delta_time, const UIContext &context)
{
    (void)delta_time;
    (void)context;
}

void BackGroundWidget::OnRender()
{
    DrawRectangleRec(bounds_, background_color_);
}
} // namespace chroma::client::ui::widget