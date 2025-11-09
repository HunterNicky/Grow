#pragma once
#include "AnimationState.h"

#include <string>
#include <unordered_map>

namespace chroma::client::render::animation {
class AnimationController
{
public:
  void LoadAnimation(const std::string &name, const Animation &animation);
  void Play(const std::string &name, bool restart = false);
  void Update(int delta_time);

  [[nodiscard]] const AnimationFrame *GetCurrentFrame() const;
  [[nodiscard]] bool IsPlaying(const std::string &name) const;

  void SetSpeed(float speed);
  void Pause();
  void Resume();

private:
  std::unordered_map<std::string, Animation> animations_;
  std::string current_animation_;
  AnimationState state_;
  bool paused_ = false;
};
}// namespace chroma::client::render::animation