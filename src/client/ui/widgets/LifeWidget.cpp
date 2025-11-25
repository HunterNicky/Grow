#include "chroma/client/ui/widgets/LifeWidget.h"
#include "chroma/shared/context/GameContext.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/components/Health.h"
#include <raylib.h>
#include <string>

namespace chroma::client::ui::widget {
// Referência ao GCM (Game Context Manager) simplificada
using GCM = shared::context::GameContextManager;
using GameContextType = shared::context::GameContextType;

LifeWidget::LifeWidget(const std::string& id, Rectangle bounds)
    : Widget(id, bounds)
{
}

void LifeWidget::OnUpdate(const float delta_time, const UIContext &context)
{
    (void)delta_time;
    (void)context;
}

void LifeWidget::OnRender()
{
    const auto client_context = GCM::Instance().GetContext(GameContextType::Client);
    if (!client_context) { return; }

    const auto player = client_context->GetLocalPlayer();
    if (!player) { return; }

    const auto health = player->GetComponent<shared::core::component::Health>();
    if (!health) { return; }

    const float current_health = *health->GetCurrentHealth();
    const float max_health = *health->GetMaxHealth();

    const float health_ratio = (max_health > 0.0F) ? (current_health / max_health) : 0.0F;
    const Rectangle bounds = GetBounds();

    const float border_size = 4.0F;
    const float padding_inside = 2.0F;

    const auto r = static_cast<unsigned char>(255.0F * (1.0F - health_ratio));
    
    const auto g = static_cast<unsigned char>(255.0F * health_ratio);
    
    const unsigned char b = 0;

    const Color fill_color = { r, g, b, 255 };

    const Color text_color = (health_ratio < 0.5F) ? BLACK : WHITE;
    
    const float inner_width = bounds.width - (2 * border_size);
    const float inner_height = bounds.height - (2 * border_size);
    const float filled_width = inner_width * health_ratio;

    DrawRectangleRec(bounds, BLACK);
    
    DrawRectangle(
        static_cast<int>(bounds.x + border_size), 
        static_cast<int>(bounds.y + border_size), 
        static_cast<int>(inner_width), 
        static_cast<int>(inner_height), 
        GRAY
    );

    DrawRectangle(
        static_cast<int>(bounds.x + border_size), 
        static_cast<int>(bounds.y + border_size), 
        static_cast<int>(filled_width), 
        static_cast<int>(inner_height), 
        fill_color
    );
    
    // 4. Desenha o texto (Vida Atual / Vida Máxima)
    const std::string health_text = std::to_string(static_cast<int>(current_health)) + 
                                    " / " + 
                                    std::to_string(static_cast<int>(max_health));
    
    // Centraliza o texto verticalmente dentro da barra
    const int font_size = 20;
    const float text_x = bounds.x + (bounds.width / 2.0F) - (static_cast<float>(MeasureText(health_text.c_str(), font_size)) / 2.0F);
    const float text_y = bounds.y + (bounds.height / 2.0F) - (font_size / 2.0F);

    DrawText(health_text.c_str(), 
             static_cast<int>(text_x), 
             static_cast<int>(text_y), 
             font_size, 
             text_color);
}

} // namespace chroma::client::ui::widget