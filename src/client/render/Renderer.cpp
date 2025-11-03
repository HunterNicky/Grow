#include "chroma/client/render/Renderer.h"
#include "chroma/client/render/Camera.h"
#include "chroma/client/render/RenderQueue.h"
#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/TextureAtlas.h"
#include "chroma/client/render/Window.h"
#include "chroma/client/render/animation/AnimationRenderer.h"

#include <functional>
#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

namespace chroma::client::render {

Renderer::Renderer(std::unique_ptr<Window> window, const RenderConfig &config)
  : window_(std::move(window)), config_(config), clear_color_(BLACK)
{
  InitializeSubsystems();
}

Renderer::~Renderer() = default;

void Renderer::BeginFrame() const
{
  BeginDrawing();
  ClearBackground(clear_color_);

  render_target_->Begin();
  ClearBackground(clear_color_);

  BeginMode2D(camera_->GetCamera2D());
}

void Renderer::EndFrame() const
{
  render_queue_->Sort();
  render_queue_->Execute();
  render_queue_->Clear();

  EndMode2D();

  RenderTarget::End();

  // const auto size = window_->GetVirtualSize();
  const int screen_w = GetScreenWidth();
  const int screen_h = GetScreenHeight();
  render_target_->Draw(screen_w, screen_h);

  EndDrawing();
}

void Renderer::RenderFrame(const std::function<void()> &draw_world) const
{
  if (window_) { window_->Update(); }
  BeginFrame();
  if (draw_world) { draw_world(); }
  EndFrame();
}

void Renderer::SetDisplayMode(const DisplayMode mode) const
{
  if (!window_) { return; }
  window_->SetDisplayMode(mode);
}

void Renderer::SetVSync(const bool enabled)
{
  config_.vsync = enabled;
  SetTargetFPS(enabled ? 0 : config_.target_fps);
}

void Renderer::SetClearColor(const Color color) { clear_color_ = color; }

void Renderer::LoadSprite(const std::string &filepath) const
{
  if (atlas_manager_) { atlas_manager_->AddImage(filepath); }
}

void Renderer::InitializeSubsystems()
{
  if (!window_) { return; }

  const int vw = config_.virtual_width > 0 ? config_.virtual_width : window_->GetVirtualSize().width;
  const int vh = config_.virtual_height > 0 ? config_.virtual_height : window_->GetVirtualSize().height;
  window_->SetVirtualResolution(vw, vh);

  render_target_ = std::make_unique<RenderTarget>(vw, vh);

  camera_ = std::make_unique<Camera>(0.0F, 0.0F, vw, vh);
  render_queue_ = std::make_unique<RenderQueue>();
  atlas_manager_ = std::make_unique<TextureAtlas>();
  sprite_renderer_ = std::make_unique<SpriteRenderer>(
    std::shared_ptr<TextureAtlas>(atlas_manager_.get(), [](TextureAtlas *) {}));
  animation_renderer_ = std::make_unique<animation::AnimationRenderer>(
    std::shared_ptr<TextureAtlas>(atlas_manager_.get(), [](TextureAtlas *) {}),
    std::shared_ptr<SpriteRenderer>(sprite_renderer_.get(), [](SpriteRenderer *) {}));

  SetTextureFilter(render_target_->GetTexture().texture, config_.filter);

  SetVSync(config_.vsync);
}
}// namespace chroma::client::render
