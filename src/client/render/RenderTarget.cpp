#include "chroma/client/render/RenderTarget.h"
#include "chroma/client/render/shader/ShaderData.h"

#include <algorithm>
#include <raylib.h>

namespace chroma::client::render {
RenderTarget::RenderTarget(const int width, const int height)
  : target_(), width_(width), height_(height), source_rec_(), dest_rec_()
{
  target_ = LoadRenderTexture(width_, height_);
  post_process_buffer_a_ = LoadRenderTexture(width_, height_);
  post_process_buffer_b_ = LoadRenderTexture(width_, height_);

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

void RenderTarget::Draw(const Texture2D& texture, const int screen_width, const int screen_height)
{
  CalculateLetterbox(screen_width, screen_height);
  DrawTexturePro(texture, source_rec_, dest_rec_, { 0.0F, 0.0F }, 0.0F, WHITE);
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

void RenderTarget::RunPostProcessPipeline(const std::vector<shader::ShaderData*>& active_shaders)
{
    Texture2D source_texture = target_.texture;

    RenderTexture2D buffers[2] = { post_process_buffer_a_, post_process_buffer_b_ };
    int dest_index = 0;

    for (size_t i = 0; i < active_shaders.size(); ++i)
    {
        shader::ShaderData* shader_data = active_shaders[i];
        
        RenderTexture2D destination_buffer = buffers[dest_index];

        BeginTextureMode(destination_buffer);
        ClearBackground(BLANK);

        BeginShaderMode(shader_data->GetShader());
        shader_data->UploadAll();

        DrawTexture(source_texture, 0, 0, WHITE);

        EndShaderMode();
        EndTextureMode();

        source_texture = destination_buffer.texture;

        dest_index = 1 - dest_index;
    }

    BeginTextureMode(target_);
    ClearBackground(BLANK);
    DrawTexture(source_texture, 0, 0, WHITE);
    EndTextureMode();
}

}// namespace chroma::client::render