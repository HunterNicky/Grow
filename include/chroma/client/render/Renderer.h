#pragma once
#include "chroma/client/render/Camera.h"
#include "chroma/client/render/RenderQueue.h"
#include "chroma/client/render/RenderTarget.h"
#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/TextureAtlas.h"
#include "chroma/client/render/Window.h"
#include "chroma/client/render/animation/AnimationRenderer.h"
#include "chroma/client/render/shader/RenderPipeline.h"
#include "chroma/client/render/shader/RenderPass.h"

#include <functional>
#include <memory>
#include <string>

namespace chroma::client::render {
  
class Renderer
{
public:
  Renderer(const Renderer &) = delete;
  Renderer(Renderer &&) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer &operator=(Renderer &&) = delete;
  Renderer(std::unique_ptr<Window> window, const RenderConfig &config);
  ~Renderer();

  void BeginFrame() const;
  void EndFrame() const;

  void RenderFrame(const std::function<void()> &draw_world) const;

  void SetDisplayMode(DisplayMode mode) const;
  void SetVSync(bool enabled);
  void SetClearColor(Color color);

  void LoadSprite(const std::string &filepath) const;

  [[nodiscard]] static bool ShouldClose() { return Window::ShouldClose(); }
  void Close()
  {
    atlas_manager_.reset();
    render_target_.reset();
    Window::Close();
  }

  void AddShaderPassFront(std::unique_ptr<shader::RenderPass> pass);
  void AddShaderPassBack(std::unique_ptr<shader::RenderPass> pass);

  [[nodiscard]] Camera &GetCamera() const { return *camera_; }
  [[nodiscard]] RenderQueue &GetQueue() const { return *render_queue_; }
  [[nodiscard]] TextureAtlas &GetAtlasManager() const { return *atlas_manager_; }
  [[nodiscard]] SpriteRenderer &GetSpriteRenderer() const { return *sprite_renderer_; }
  [[nodiscard]] animation::AnimationRenderer &GetAnimationRenderer() const { return *animation_renderer_; }

private:

  std::unique_ptr<Window> window_;
  RenderConfig config_;
  Color clear_color_;

  std::unique_ptr<RenderTarget> render_target_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<RenderQueue> render_queue_;
  std::unique_ptr<TextureAtlas> atlas_manager_;
  std::unique_ptr<SpriteRenderer> sprite_renderer_;
  std::unique_ptr<animation::AnimationRenderer> animation_renderer_;
  std::unique_ptr<shader::RenderPipeline> render_pipeline_;

  void InitializeSubsystems();
};
}// namespace chroma::client::render
