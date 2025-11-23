#include "chroma/shared/events/ProjectileEvent.h"
#include "chroma/shared/events/Event.h"
#include <raylib.h>

namespace chroma::shared::event {

ProjectileEvent::ProjectileEvent(core::component::TypeProjectile type, const Vector2 &direction, float speed)
  : Event(Event::Type::ProjectileEvent), projectile_type_(type), direction_(direction), speed_(speed)
{}

ProjectileEvent::ProjectileEvent() : Event(Event::Type::ProjectileEvent) {}

core::component::TypeProjectile ProjectileEvent::GetProjectileType() const { return projectile_type_; }

ProjectileEvent::Type ProjectileEvent::GetStaticType() { return Event::Type::ProjectileEvent; }

std::shared_ptr<Event> ProjectileEvent::Clone() const { return std::make_shared<ProjectileEvent>(*this); }

const Vector2 &ProjectileEvent::GetDirection() const { return direction_; }

float ProjectileEvent::GetSpeed() const { return speed_; }

const UUIDv4::UUID &ProjectileEvent::GetProjectileId() const { return projectile_id_; }

void ProjectileEvent::SetProjectileId(const UUIDv4::UUID &id) { projectile_id_ = id; }

const Vector2 &ProjectileEvent::GetPosition() const { return position_; }

void ProjectileEvent::SetPosition(const Vector2 &position) { position_ = position; }

}// namespace chroma::shared::event