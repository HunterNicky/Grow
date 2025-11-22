#pragma once

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
enum class TypeProjectile : uint8_t {
    ARROW = 0,
    JAVELIN = 1,
};

class ProjectileType : public Component {
public:
    ProjectileType();
    explicit ProjectileType(TypeProjectile projectile_type);
    
    ~ProjectileType() override = default;

    ProjectileType(const ProjectileType &) = default;
    ProjectileType(ProjectileType &&) = delete;
    ProjectileType &operator=(const ProjectileType &) = default;
    ProjectileType &operator=(ProjectileType &&) = delete;

    [[nodiscard]] TypeProjectile GetProjectileType() const;

    void SetProjectileType(TypeProjectile projectile_type);
private:
    TypeProjectile projectile_type_{ TypeProjectile::ARROW };
};

} // namespace chroma::shared::core::component