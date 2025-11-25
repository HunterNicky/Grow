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
    const Color text_color = WHITE;
    
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
    
    const std::string level_text = "Nível " + std::to_string(static_cast<int>(current_level));
    const int level_font_size = 24;
    const float level_text_x = bounds.x + border_size + 10; 
    const float level_text_y = bounds.y + (bounds.height / 2.0F) - (level_font_size / 2.0F);
    
    DrawText(level_text.c_str(), 
             static_cast<int>(level_text_x), 
             static_cast<int>(level_text_y), 
             level_font_size, 
             text_color);

    const std::string xp_progress_text = std::to_string(static_cast<int>(current_xp)) + 
                                         " / " + 
                                         std::to_string(static_cast<int>(xp_to_next_level));
    
    const int progress_font_size = 24;
    auto text_width = static_cast<float>(MeasureText(xp_progress_text.c_str(), progress_font_size));
    
    const float progress_text_x = bounds.x + bounds.width - border_size - 10 - text_width;
    const float progress_text_y = bounds.y + (bounds.height / 2.0F) - (progress_font_size / 2.0F);

    DrawText(xp_progress_text.c_str(), 
             static_cast<int>(progress_text_x), 
             static_cast<int>(progress_text_y), 
             progress_font_size, 
             text_color);
}

} // namespace chroma::client::ui::widget