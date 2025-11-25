#include "chroma/client/ui/widgets/LifeWidget.h"
#include "chroma/shared/context/GameContextManager.h"
#include "chroma/shared/core/components/Health.h"

namespace chroma::client::ui::widget {
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
    const auto player = GCM::Instance().GetContext(GameContextType::Client)->GetLocalPlayer();
    if (!player) { return; }

    const auto health = player->GetComponent<shared::core::component::Health>();
    if (!health) { return; }

    const float health_ratio = *health->GetCurrentHealth() / *health->GetMaxHealth();
    const Rectangle bounds = GetBounds();

    DrawRectangleLinesEx(bounds, 2.0F, BLACK);
    DrawRectangleRec(
        { bounds.x + 2.0F, bounds.y + 2.0F, (bounds.width - 4.0F) * health_ratio, bounds.height - 4.0F }, RED
    );
}

} // namespace chroma::client::ui::widget