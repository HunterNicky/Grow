#include "chroma/shared/core/components/ProjectileType.h"

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {
ProjectileType::ProjectileType(TypeProjectile projectile_type) : projectile_type_(projectile_type)
{
  type_ = ComponentType::PROJECTILE_TYPE;
}

ProjectileType::ProjectileType() { type_ = ComponentType::PROJECTILE_TYPE; }

TypeProjectile ProjectileType::GetProjectileType() const { return projectile_type_; }

void ProjectileType::SetProjectileType(TypeProjectile projectile_type) { projectile_type_ = projectile_type; }

}// namespace chroma::shared::core::component