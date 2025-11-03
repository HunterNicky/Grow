#include "chroma/client/render/RenderBridgeImpl.h"
#include "chroma/client/render/Camera.h"
#include "chroma/client/render/RenderQueue.h"
#include "chroma/client/render/Renderer.h"
#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/animation/AnimationController.h"
#include "chroma/client/render/animation/AnimationRenderer.h"
#include "chroma/client/render/animation/AnimationState.h"
#include "chroma/shared/core/components/SpriteAnimation.h"
#include "chroma/shared/render/RenderBridge.h"

#include <numeric>
#include <raylib.h>
#include <string>

namespace chroma::client::render {

RenderBridgeImpl::RenderBridgeImpl(Renderer *renderer) : renderer_(renderer) {}

void RenderBridgeImpl::DrawSprite(const std::string &sprite_id,
  const Vector2 position,
  const Vector2 scale,
  const float rotation,
  const Color tint,
  const bool flip_x,
  const bool flip_y,
  const Vector2 origin,
  const Vector2 offset)
{
  if (renderer_ == nullptr) { return; }

  SpriteDrawParams params;
  params.position = { .x = position.x + offset.x, .y = position.y + offset.y };
  params.scale = scale;
  params.rotation = rotation;
  params.tint = tint;
  params.flip_x = flip_x;
  params.flip_y = flip_y;
  params.origin = origin;

  auto &queue = renderer_->GetQueue();
  auto &sprites = renderer_->GetSpriteRenderer();
  // NOLINTNEXTLINE(bugprone-exception-escape)
  queue.Submit(RenderLayer::Entities, 0, [=, &sprites]() noexcept { sprites.DrawSprite(sprite_id, params); });
}

void RenderBridgeImpl::DrawAnimation(const shared::core::component::SpriteAnimation &anim,
  const Vector2 position,
  const Vector2 scale,
  const float rotation,
  const Color tint,
  const bool flip_x,
  const bool flip_y,
  const Vector2 origin)
{
  if (renderer_ == nullptr) { return; }

  const auto *key = static_cast<const void *>(&anim);
  auto &ctrl = controllers_[key];

  if (!initialized_.contains(key)) {
    for (const auto &entry : anim.GetAnimations()) {
      const auto &name = entry.first;
      const auto &desc = entry.second;
      animation::Animation tmp;
      tmp.name = desc.name;
      tmp.loop = desc.loop;
      tmp.total_duration = desc.total_duration;
      tmp.frames.reserve(desc.frames.size());
      for (const auto &f : desc.frames) {
        animation::AnimationFrame af{};
        af.sprite_id = f.sprite_id;
        af.duration_ticks = f.duration_ticks;
        af.offset = f.offset;
        af.subregion = f.subregion;
        af.subregion_offset = f.subregion_offset;
        tmp.frames.push_back(af);
      }
      if (tmp.total_duration <= 0) {
        // NOLINTNEXTLINE
        tmp.total_duration = std::accumulate(
          tmp.frames.begin(), tmp.frames.end(), 0, [](int sum, const auto &f) { return sum + f.duration_ticks; });
      }
      ctrl.LoadAnimation(name, tmp);
    }
    initialized_.insert(key);
  }

  const std::string &desired = anim.GetCurrentAnimationName();
  if (!desired.empty()) {
    if (!current_anim_.contains(key) || current_anim_[key] != desired) {
      ctrl.Play(desired, true);
      current_anim_[key] = desired;
    }
  }
  ctrl.SetSpeed(anim.GetSpeed());

  if (anim.IsPaused()) {
    ctrl.Pause();
  } else {
    ctrl.Resume();
  }

  const int delta_ticks = static_cast<int>(GetFrameTime() * 1000.0F);
  ctrl.Update(delta_ticks);

  SpriteDrawParams params;
  params.position = position;
  params.scale = scale;
  params.rotation = rotation;
  params.tint = tint;
  params.flip_x = flip_x;
  params.flip_y = flip_y;
  params.origin = origin;

  auto &queue = renderer_->GetQueue();
  auto &anim_renderer = renderer_->GetAnimationRenderer();
  // NOLINTNEXTLINE(bugprone-exception-escape)
  queue.Submit(
    RenderLayer::Entities, 0, [=, &anim_renderer, &ctrl]() noexcept { anim_renderer.DrawAnimation(ctrl, params); });
}

void RenderBridgeImpl::LoadSprite(const std::string &filepath)
{
  if (renderer_ != nullptr) { renderer_->LoadSprite(filepath); }
}

static CameraMode MapMode(const shared::render::CameraMode mode)
{
  using SM = shared::render::CameraMode;
  using CM = CameraMode;
  switch (mode) {
  case SM::Free:
    return CM::Free;
  case SM::Follow:
    return CM::Follow;
  case SM::FollowSmooth:
    return CM::FollowSmooth;
  case SM::Bounded:
    return CM::Bounded;
  }
  return CM::Free;
}

void RenderBridgeImpl::CameraSetMode(const shared::render::CameraMode mode)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().SetMode(MapMode(mode));
}

void RenderBridgeImpl::CameraSetTarget(const Vector2 target)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().SetTarget(target);
}

void RenderBridgeImpl::CameraSetBounds(const Rectangle bounds)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().SetBounds(bounds);
}

void RenderBridgeImpl::CameraSetZoom(const float zoom)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().SetZoom(zoom);
}

void RenderBridgeImpl::CameraSetSmoothness(const float smoothness)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().SetSmoothness(smoothness);
}

void RenderBridgeImpl::CameraUpdate(const float delta_time)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().Update(delta_time);
}

void RenderBridgeImpl::CameraSetDeadzone(const Vector2 size)
{
  if (renderer_ == nullptr) { return; }
  renderer_->GetCamera().SetDeadzone(size.x, size.y);
}

}// namespace chroma::client::render
