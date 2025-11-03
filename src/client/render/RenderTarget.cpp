#include "chroma/client/render/RenderTarget.h"

#include <raylib.h>
#include <algorithm>

namespace chroma::client::render {
RenderTarget::RenderTarget(const int width, const int height)
  : target_(), width_(width), height_(height), source_rec_(), dest_rec_()
{
  target_ = LoadRenderTexture(width_, height_);

  source_rec_ = { .x = 0.0F, .y = 0.0F, .width = static_cast<float>(width_), .height = -static_cast<float>(height_) };
  dest_rec_ = { .x = 0.0F, .y = 0.0F, .width = static_cast<float>(width_), .height = static_cast<float>(height_) };
}

RenderTarget::~RenderTarget()
{
  if (target_.id != 0) {
    UnloadRenderTexture(target_);
    target_ = {};
  }
}

void RenderTarget::Begin() const { BeginTextureMode(target_); }

void RenderTarget::End() { EndTextureMode(); }

void RenderTarget::Draw(const int screen_width, const int screen_height)
{
  CalculateLetterbox(screen_width, screen_height);
  DrawTexturePro(target_.texture, source_rec_, dest_rec_, { 0.0F, 0.0F }, 0.0F, WHITE);
}

void RenderTarget::CalculateLetterbox(const int screen_width, const int screen_height)
{
  if (screen_width <= 0 || screen_height <= 0 || width_ <= 0 || height_ <= 0) { return; }

  const auto sw = static_cast<float>(screen_width);
  const auto sh = static_cast<float>(screen_height);
  const auto vw = static_cast<float>(width_);
  const auto vh = static_cast<float>(height_);

  const float scale = std::min(sw / vw, sh / vh);
  const float dw = vw * scale;
  const float dh = vh * scale;
  const float dx = (sw - dw) * 0.5F;
  const float dy = (sh - dh) * 0.5F;

  source_rec_ = { .x = 0.0F, .y = 0.0F, .width = vw, .height = -vh };
  dest_rec_ = { .x = dx, .y = dy, .width = dw, .height = dh };
}
}// namespace chroma::client::render