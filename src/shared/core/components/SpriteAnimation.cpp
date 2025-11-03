#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/core/components/Component.h"

#include <string>
#include <unordered_map>

namespace chroma::shared::core::component {

SpriteAnimation::SpriteAnimation() { type_ = ComponentType::SPRITE_ANIMATION; }

void SpriteAnimation::LoadAnimation(const std::string &name, const SpriteAnimationDesc &anim_desc)
{
  animations_[name] = anim_desc;
}

void SpriteAnimation::Play(const std::string &name, const bool restart)
{
  if (!restart && current_animation_ == name) { return; }
  if (!animations_.contains(name)) { return; }
  current_animation_ = name;
  paused_ = false;
}

void SpriteAnimation::SetSpeed(const float speed) { speed_ = speed; }

void SpriteAnimation::Pause() { paused_ = true; }

void SpriteAnimation::Resume() { paused_ = false; }

const std::unordered_map<std::string, SpriteAnimationDesc> &SpriteAnimation::GetAnimations() const
{
  return animations_;
}

const std::string &SpriteAnimation::GetCurrentAnimationName() const { return current_animation_; }

float SpriteAnimation::GetSpeed() const { return speed_; }

bool SpriteAnimation::IsPaused() const { return paused_; }

}// namespace chroma::shared::core::component
