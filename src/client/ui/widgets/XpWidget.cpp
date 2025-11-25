#include "chroma/client/ui/widgets/XpWidget.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/components/Nivel.h"
#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {
using GCM = shared::context::GameContextManager;
using GameContextType = shared::context::GameContextType;
using Nivel = shared::core::component::Nivel;

XpWidget::XpWidget(const std::string& id, Rectangle bounds)
    : Widget(id, bounds)
{
    const float border_size = 3.0F;
    const int font_size = 24;
    const float inner_x = bounds.x + border_size;
    const float inner_y = bounds.y + border_size;
    const float inner_width = bounds.width - (2 * border_size);
    const float inner_height = bounds.height - (2 * border_size);

    const Rectangle level_text_bounds = { 
        inner_x, 
        inner_y, 
        inner_width / 2.0F, 
        inner_height 
    };

    level_text_widget_ = std::make_unique<TextWidget>(
        id + "_level", level_text_bounds, "Nível 1", font_size, WHITE);
    
    const Rectangle progress_text_bounds = { 
        inner_x + (inner_width / 2.0F),
        inner_y, 
        inner_width / 2.0F,             
        inner_height 
    };
    progress_text_widget_ = std::make_unique<TextWidget>(
        id + "_progress", progress_text_bounds, "0 / 100", font_size, WHITE);
}

void XpWidget::OnUpdate(const float delta_time, const UIContext &context)
{
    (void)delta_time;
    (void)context;
}

void XpWidget::OnRender()
{
    const auto client_context = GCM::Instance().GetContext(GameContextType::Client);
    if (!client_context) { return; }

    const auto player = client_context->GetLocalPlayer();
    if (!player) { return; }

    const auto nivel = player->GetComponent<Nivel>();
    if (!nivel) { return; }

    const float current_xp = nivel->GetExperience();
    const float xp_to_next_level = nivel->GetExperienceToNextNivel();
    const int current_level = nivel->GetNivel(); 

    const float xp_ratio = (xp_to_next_level > 0.0F) ? (current_xp / xp_to_next_level) : 0.0F;
    const Rectangle bounds = GetBounds();

    const float border_size = 3.0F;
    const Color fill_color = { 0, 191, 255, 255 };
    
    const float inner_width = bounds.width - (2 * border_size);
    const float inner_height = bounds.height - (2 * border_size);
    const float filled_width = inner_width * xp_ratio;

    DrawRectangleRec(bounds, BLACK);
    
    DrawRectangle(
        static_cast<int>(bounds.x + border_size), 
        static_cast<int>(bounds.y + border_size), 
        static_cast<int>(inner_width), 
        static_cast<int>(inner_height), 
        DARKGRAY 
    );

    DrawRectangle(
        static_cast<int>(bounds.x + border_size), 
        static_cast<int>(bounds.y + border_size), 
        static_cast<int>(filled_width), 
        static_cast<int>(inner_height), 
        fill_color
    );
    
    const std::string level_text = "Nível " + std::to_string(current_level);
    level_text_widget_->SetText(level_text);
    level_text_widget_->OnRender();

    const std::string xp_progress_text = std::to_string(static_cast<int>(current_xp)) + 
                                         " / " + 
                                         std::to_string(static_cast<int>(xp_to_next_level));
    
    progress_text_widget_->SetText(xp_progress_text);
    progress_text_widget_->OnRender();
}

} // namespace chroma::client::ui::widget