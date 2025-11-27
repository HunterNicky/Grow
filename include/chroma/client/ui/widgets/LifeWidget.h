#pragma once

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {

class LifeWidget : public Widget
{
public:
    LifeWidget(const std::string& id, Rectangle bounds);

    void OnUpdate(const float delta_time, const UIContext &context) override;
    void OnRender() override;
};

} // namespace chroma::client::ui::widget