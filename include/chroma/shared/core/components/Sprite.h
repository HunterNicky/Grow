#pragma once

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>
#include <string>

namespace chroma::shared::core::component {
class Sprite : public Component
{
public:
  Sprite();
  Sprite(const Sprite &) = default;
  Sprite(Sprite &&) = delete;
  Sprite &operator=(const Sprite &) = default;
  Sprite &operator=(Sprite &&) = delete;
  ~Sprite() override = default;

  void SetSpriteId(const std::string &id);
  [[nodiscard]] const std::string &GetSpriteId() const;

  void SetTint(Color tint);
  [[nodiscard]] Color GetTint() const;

  void SetFlipX(bool flip);
  void SetFlipY(bool flip);
  [[nodiscard]] bool GetFlipX() const;
  [[nodiscard]] bool GetFlipY() const;

private:
  std::string sprite_id_;
  Color tint_;
  bool flip_x_;
  bool flip_y_;
};
}// namespace chroma::shared::core::component
