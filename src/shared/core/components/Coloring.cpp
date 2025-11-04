#include "chroma/shared/core/components/Coloring.h"
#include "chroma/shared/core/components/Component.h"

#include <cstdint>

namespace chroma::shared::core::component {

Coloring::Coloring(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : color_{ r, g, b, a }
{
  type_ = ComponentType::COLOR;
}

uint8_t Coloring::GetRed() const { return color_.r; }

uint8_t Coloring::GetGreen() const { return color_.g; }

uint8_t Coloring::GetBlue() const { return color_.b; }

uint8_t Coloring::GetAlpha() const { return color_.a; }

void Coloring::SetRed(uint8_t r) { color_.r = r; }

void Coloring::SetGreen(uint8_t g) { color_.g = g; }

void Coloring::SetBlue(uint8_t b) { color_.b = b; }

void Coloring::SetAlpha(uint8_t a) { color_.a = a; }

void Coloring::SetColoring(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  color_ = Color{r,g,b, a};
}

Color Coloring::GetColor() const
{
  return color_;
}

}// namespace chroma::shared::core::component