#pragma once

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace chroma::shared::core::component {

struct SpriteAnimFrame
{
  std::string sprite_id;
  int duration_ticks = 0;
  Vector2 offset{ 0, 0 };
  Rectangle subregion{ 0, 0, 0, 0 };
  Vector2 subregion_offset{ 0, 0 };
};

struct SpriteAnimationDesc
{
  std::string name;
  std::vector<SpriteAnimFrame> frames;
  bool loop = true;
  int total_duration = 0;
};

class SpriteAnimation final : public Component
{
public:
  SpriteAnimation();
  SpriteAnimation(const SpriteAnimation &) = default;
  SpriteAnimation(SpriteAnimation &&) = delete;
  SpriteAnimation &operator=(const SpriteAnimation &) = default;
  SpriteAnimation &operator=(SpriteAnimation &&) = delete;
  ~SpriteAnimation() override = default;

  void LoadAnimation(const std::string &name, const SpriteAnimationDesc &anim_desc);
  void Play(const std::string &name, bool restart = false);

  void SetSpeed(float speed);
  void Pause();
  void Resume();

  [[nodiscard]] const std::unordered_map<std::string, SpriteAnimationDesc> &GetAnimations() const;
  [[nodiscard]] const std::string &GetCurrentAnimationName() const;
  [[nodiscard]] float GetSpeed() const;
  [[nodiscard]] bool IsPaused() const;

private:
  std::unordered_map<std::string, SpriteAnimationDesc> animations_;
  std::string current_animation_;

  float speed_ = 1.0F;
  bool paused_ = false;
};
}// namespace chroma::shared::core::component
