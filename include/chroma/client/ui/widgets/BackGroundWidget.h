#pragma once

#include "chroma/client/ui/widgets/Widget.h"
#include <raylib.h>

namespace chroma::client::ui::widget {
class BackGroundWidget : public Widget {
public:
    BackGroundWidget(const std::string& id, Rectangle bounds, Color color = GRAY);
    void OnUpdate(const float delta_time, const UIContext &context) override;
    void OnRender() override;
private:
    Color background_color_;
};
} // namespace chroma::client::ui::widget