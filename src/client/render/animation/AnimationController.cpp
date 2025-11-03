#include "chroma/client/render/animation/AnimationController.h"
#include "chroma/client/render/animation/AnimationState.h"

#include <string>

namespace chroma::client::render::animation {
void AnimationController::LoadAnimation(const std::string &name, const Animation &animation)
{
  animations_[name] = animation;
}

void AnimationController::Play(const std::string &name, const bool restart)
{
  const auto it = animations_.find(name);
  if (it == animations_.end()) { return; }

  if (current_animation_ == name && !restart) { return; }

  current_animation_ = name;
  state_ = AnimationState(&it->second);
  state_.Reset();
  paused_ = false;
}

void AnimationController::Update(const int delta_time)
{
  if (paused_ || current_animation_.empty()) { return; }

  state_.Update(static_cast<float>(delta_time));
}

const AnimationFrame *AnimationController::GetCurrentFrame() const
{
  if (current_animation_.empty()) { return nullptr; }

  return &state_.GetCurrentFrame();
}

bool AnimationController::IsPlaying(const std::string &name) const
{
  return !paused_ && current_animation_ == name && !state_.IsFinished();
}

void AnimationController::SetSpeed(const float speed) { state_.SetSpeed(speed); }

void AnimationController::Pause() { paused_ = true; }

void AnimationController::Resume() { paused_ = false; }
}// namespace chroma::client::render::animation