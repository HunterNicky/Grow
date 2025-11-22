#pragma once

#include "chroma/shared/events/ProjectileEvent.h"
#include <cstdint>
#include <memory>

namespace chroma::shared::packet {
class ProjectileMessage
{
public:
  ProjectileMessage() = default;
  ProjectileMessage(uint32_t sequence, float delta_time);
  ProjectileMessage(const ProjectileMessage &) = default;
  ProjectileMessage(ProjectileMessage &&)  noexcept = default;
  ProjectileMessage &operator=(const ProjectileMessage &) = default;
  ProjectileMessage &operator=(ProjectileMessage &&)  noexcept = default;
  ~ProjectileMessage() = default;

  [[nodiscard]] uint32_t GetSeq() const;
  [[nodiscard]] float GetDeltaTime() const;

  void SetSeq(uint32_t seq);
  void SetDeltaTime(float dt);

  [[nodiscard]] const std::shared_ptr<event::ProjectileEvent> &GetProjectileEvent() const;
  std::shared_ptr<event::ProjectileEvent> &GetProjectileEvent();

private:
    uint32_t seq_{};
    float dt_{};
    std::shared_ptr<event::ProjectileEvent> projectile_event_;
};
}// namespace chroma::shared::packet
