#pragma once
#include <raylib.h>
#include <string>
#include <vector>

namespace chroma::client::render::animation {
struct AnimationFrame
{
  std::string sprite_id;
  int duration_ticks;
  Vector2 offset = { 0, 0 };
  Rectangle subregion = { 0, 0, 0, 0 };
  Vector2 subregion_offset = { 0, 0 };
};

struct Animation
{
  std::string name;
  std::vector<AnimationFrame> frames;
  bool loop = true;
  int total_duration = 0;
};

class AnimationState
{
public:
  AnimationState() = default;
  explicit AnimationState(const Animation *anim);

  void Update(float delta_time);
  void Reset();
  void SetSpeed(const float speed);

  [[nodiscard]] const AnimationFrame &GetCurrentFrame() const;
  [[nodiscard]] bool IsFinished() const;
  [[nodiscard]] int GetCurrentFrameIndex() const;

private:
  const Animation *animation_ = nullptr;
  int current_tick_ = 0;
  int current_frame_index_ = 0;
  float speed_ = 1.0F;
  bool finished_ = false;

  void CalculateCurrentFrame();
};
}// namespace chroma::client::render::animation
