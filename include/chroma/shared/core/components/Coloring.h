#pragma once

#include "chroma/shared/core/components/Component.h"

#include <raylib.h>

namespace chroma::shared::core::component {
class Coloring : public Component
{
public:
  explicit Coloring(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  Coloring() = default;
  Coloring(const Coloring &) = default;
  Coloring(Coloring &&) = delete;
  Coloring &operator=(const Coloring &) = default;
  Coloring &operator=(Coloring &&) = delete;
  ~Coloring() override = default;

  [[nodiscard]] uint8_t GetRed() const;
  [[nodiscard]] uint8_t GetGreen() const;
  [[nodiscard]] uint8_t GetBlue() const;
  [[nodiscard]] uint8_t GetAlpha() const;

  void SetRed(uint8_t r);
  void SetGreen(uint8_t g);
  void SetBlue(uint8_t b);
  void SetAlpha(uint8_t a);

  void SetColoring(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  [[nodiscard]] Color GetColor() const;

private:
  Color color_{};
};
}// namespace chroma::shared::core::component
