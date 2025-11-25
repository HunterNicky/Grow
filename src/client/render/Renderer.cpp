#include "chroma/client/render/Renderer.h"
#include "chroma/client/render/Camera.h"
#include "chroma/client/render/RenderQueue.h"
#include "chroma/client/render/SpriteRenderer.h"
#include "chroma/client/render/TextureAtlas.h"
#include "chroma/client/render/Window.h"
#include "chroma/client/render/animation/AnimationRenderer.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/RenderPipeline.h"
#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ShaderEvent.h"
#include "chroma/client/factory/ShaderFactory.h"
#include "chroma/client/ui/UIManagerBus.h"

#include <functional>
#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

namespace chroma::client::render {

Renderer::Renderer(std::unique_ptr<Window> window, const RenderConfig &config)
  : window_(std::move(window)), config_(config), clear_color_(WHITE)
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

  
  RenderTexture2D scene_rt = render_target_->GetTexture();
  RenderTexture2D processed_rt = render_pipeline_->Execute(scene_rt);
  
  BeginTextureMode(processed_rt);
  ui::UIManagerBus::GetUIManager()->OnRender();
  EndTextureMode();

  render_target_->Draw(processed_rt.texture, GetScreenWidth(), GetScreenHeight());

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

  camera_ = std::make_unique<Camera>(static_cast<float>(vh) / 2.0F, static_cast<float>(vw) / 2.0F, vw, vh);
  render_queue_ = std::make_unique<RenderQueue>();
  atlas_manager_ = std::make_unique<TextureAtlas>();
  sprite_renderer_ =
    std::make_unique<SpriteRenderer>(std::shared_ptr<TextureAtlas>(atlas_manager_.get(), [](TextureAtlas *) {}));
  animation_renderer_ = std::make_unique<animation::AnimationRenderer>(
    std::shared_ptr<TextureAtlas>(atlas_manager_.get(), [](TextureAtlas *) {}),
    std::shared_ptr<SpriteRenderer>(sprite_renderer_.get(), [](SpriteRenderer *) {}));

  render_pipeline_ = std::make_unique<shader::RenderPipeline>();
  render_pipeline_->Initialize(vw, vh);

  auto crt_pass = std::make_unique<shader::shaders::BorderPass>(vw, vh);
  render_pipeline_->AddPassBack(std::move(crt_pass));
  render_pipeline_->Setup();

  SetTextureFilter(render_target_->GetTexture().texture, config_.filter);
  SetVSync(config_.vsync);

}

void Renderer::AddShaderPassFront(std::unique_ptr<shader::RenderPass> pass) const
{
  if (render_pipeline_ == nullptr) { return; }
  render_pipeline_->AddPassFront(std::move(pass));
  render_pipeline_->Setup();
}

void Renderer::AddShaderPassBack(std::unique_ptr<shader::RenderPass> pass) const
{
  if (render_pipeline_ == nullptr) { return; }
  render_pipeline_->AddPassBack(std::move(pass));
  render_pipeline_->Setup();
}

void Renderer::SetEventDispatcher()
{
  key_sub_ = shared::event::EventBus::GetDispatcher()->Subscribe<shared::event::ShaderEvent>(
    [this](shared::event::Event &event) { this->HandleShaderEvent(event); });
}

void Renderer::HandleShaderEvent(const shared::event::Event &event) const
{
  const auto &shader_event = dynamic_cast<const shared::event::ShaderEvent &>(event);
  switch (shader_event.GetShaderEventType()) {
    case shared::event::ShaderEventType::ADD:
      HandleAddShaderEvent(event);
      break;
    case shared::event::ShaderEventType::CHANGE:
      HandleChangeShaderEvent(event);
      break;
    default:
      break;
  }
}

void Renderer::HandleAddShaderEvent(const shared::event::Event &event) const
{
  const auto &shader_event = dynamic_cast<const shared::event::ShaderEvent &>(event);
  std::unique_ptr<shader::RenderPass> shader_pass = factory::ShaderPassFactory::Create(shader_event.GetPassType());

  if(!shader_pass) { return; }

  if(shader_event.IsFront()) {
      AddShaderPassFront(std::move(shader_pass));
  } else {
      AddShaderPassBack(std::move(shader_pass));
  }
}

void Renderer::HandleChangeShaderEvent(const shared::event::Event &event) const
{
  const auto &shader_event = dynamic_cast<const shared::event::ShaderEvent &>(event);
  shader::RenderPass* pass = render_pipeline_->GetPassByType(shader_event.GetPassType());
  if(pass == nullptr) { return; }

  pass->SetActive(!pass->IsActive());
}

}// namespace chroma::client::render
