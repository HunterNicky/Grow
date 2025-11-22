#pragma once

#include <raylib.h>
#include <uuid_v4.h>

#include "chroma/shared/events/Event.h"
#include "chroma/shared/core/components/ProjectileType.h"

namespace chroma::shared::event {
  ;

    class ProjectileEvent : public Event
    {
    public:
        explicit ProjectileEvent(core::component::TypeProjectile type, const Vector2& direction, float speed);
        ProjectileEvent();
        ~ProjectileEvent() override = default;

        ProjectileEvent(ProjectileEvent&&) = default;
        ProjectileEvent(const ProjectileEvent&) = default;
        ProjectileEvent& operator=(const ProjectileEvent&) = default;
        ProjectileEvent& operator=(ProjectileEvent&&) = default;

        [[nodiscard]] core::component::TypeProjectile GetProjectileType() const;
        [[nodiscard]] std::shared_ptr<Event> Clone() const override;
        [[nodiscard]] static Type GetStaticType();

        [[nodiscard]] const Vector2& GetDirection() const;
        [[nodiscard]] float GetSpeed() const;

        [[nodiscard]] const UUIDv4::UUID& GetProjectileId() const;
        void SetProjectileId(const UUIDv4::UUID& id);

        [[nodiscard]] const Vector2& GetPosition() const;
        void SetPosition(const Vector2& position);

    private:
        core::component::TypeProjectile projectile_type_{ core::component::TypeProjectile::ARROW };
        Vector2 direction_{ 0.0F, 0.0F };
        Vector2 position_{ 0.0F, 0.0F };
        float speed_{ 0.0F };
        UUIDv4::UUID projectile_id_;
    };
} // namespace chroma::shared::event 