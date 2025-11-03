#include "chroma/client/render/animation/AnimationState.h"

#include <cstddef>

namespace chroma::client::render::animation {
AnimationState::AnimationState(const Animation *anim) : animation_(anim) { Reset(); }

void AnimationState::Update(const float delta_time)
{
  if ((animation_ == nullptr) || animation_->frames.empty() || finished_) { return; }

  const int delta_ticks = static_cast<int>(delta_time * speed_);
  current_tick_ += delta_ticks;

  if (animation_->loop) {
    if (animation_->total_duration > 0) { current_tick_ %= animation_->total_duration; }
    finished_ = false;
  } else {
    if (current_tick_ >= animation_->total_duration) {
      current_tick_ = animation_->total_duration;
      finished_ = true;
    }
  }

  CalculateCurrentFrame();
}

void AnimationState::Reset()
{
  current_tick_ = 0;
  current_frame_index_ = 0;
  finished_ = false;
}

void AnimationState::SetSpeed(const float speed) { speed_ = speed; }

const AnimationFrame &AnimationState::GetCurrentFrame() const
{
  static AnimationFrame const empty_frame{};
  if ((animation_ == nullptr) || animation_->frames.empty()) { return empty_frame; }
  return animation_->frames[current_frame_index_];
}

bool AnimationState::IsFinished() const { return finished_; }

int AnimationState::GetCurrentFrameIndex() const { return current_frame_index_; }

void AnimationState::CalculateCurrentFrame()
{
  if ((animation_ == nullptr) || animation_->frames.empty()) { return; }

  int accumulated = 0;
  for (size_t i = 0; i < animation_->frames.size(); ++i) {
    accumulated += animation_->frames[i].duration_ticks;
    if (current_tick_ < accumulated) {
      current_frame_index_ = static_cast<int>(i);
      return;
    }
  }

  current_frame_index_ = static_cast<int>(animation_->frames.size()) - 1;
}
}// namespace chroma::client::render::animation