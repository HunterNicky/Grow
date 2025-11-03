#include "chroma/shared/core/components/Color.h"
#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {

Color::Color(float r, float g, float b, float a) : red_(r), green_(g), blue_(b), alpha_(a)
{
  type_ = ComponentType::COLOR;
}

float Color::GetRed() const { return red_; }

float Color::GetGreen() const { return green_; }

float Color::GetBlue() const { return blue_; }

float Color::GetAlpha() const { return alpha_; }

void Color::SetRed(float r) { red_ = r; }

void Color::SetGreen(float g) { green_ = g; }

void Color::SetBlue(float b) { blue_ = b; }

void Color::SetAlpha(float a) { alpha_ = a; }

void Color::SetColor(float r, float g, float b, float a)
{
  red_ = r;
  green_ = g;
  blue_ = b;
  alpha_ = a;
}

}// namespace chroma::shared::core::component