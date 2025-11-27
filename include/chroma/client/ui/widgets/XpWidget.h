#pragma once

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"
#include "chroma/client/ui/widgets/TextWidget.h"

#include <raylib.h>
#include <string>
#include <memory>

    
namespace chroma::client::ui::widget {

class XpWidget : public Widget
{
public:
    XpWidget(const std::string& id, Rectangle bounds);

    void OnUpdate(const float delta_time, const UIContext &context) override;
    void OnRender() override;

private:
    std::unique_ptr<TextWidget> level_text_widget_;
    std::unique_ptr<TextWidget> progress_text_widget_;
};

} // namespace chroma::client::ui::widget