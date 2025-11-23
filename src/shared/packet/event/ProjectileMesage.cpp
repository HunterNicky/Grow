#include "chroma/shared/packet/events/ProjectileMessage.h"

namespace chroma::shared::packet {
ProjectileMessage::ProjectileMessage(uint32_t sequence, float delta_time) : seq_(sequence), dt_(delta_time) {}

uint32_t ProjectileMessage::GetSeq() const { return seq_; }
float ProjectileMessage::GetDeltaTime() const { return dt_; }

void ProjectileMessage::SetSeq(uint32_t seq) { seq_ = seq; }
void ProjectileMessage::SetDeltaTime(float dt) { dt_ = dt; }

const std::shared_ptr<event::ProjectileEvent> &ProjectileMessage::GetProjectileEvent() const
{
  return projectile_event_;
}
std::shared_ptr<event::ProjectileEvent> &ProjectileMessage::GetProjectileEvent() { return projectile_event_; }

}// namespace chroma::shared::packet