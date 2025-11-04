#include "chroma/client/render/TextureAtlas.h"

#include <algorithm>
#include <optional>
#include <raylib.h>
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <string>
#include <vector>

namespace chroma::client::render {
TextureAtlas::TextureAtlas(const int width, const int height)
  : width_(width), height_(height), dirty_(false), texture_()
{}

TextureAtlas::~TextureAtlas() { ClearAtlas(); }

void TextureAtlas::AddImage(const std::string &filepath)
{
  if (std::ranges::find(image_files_, filepath) != image_files_.end()) { return; }
  image_files_.push_back(filepath);
  dirty_ = true;
}

std::optional<const AtlasRegion *> TextureAtlas::GetRegion(const std::string &id)
{
  if (dirty_) { Rebuild(); }

  const auto it = regions_.find(id);
  if (it != regions_.end()) { return &it->second; }
  return std::nullopt;
}

const Texture *TextureAtlas::GetTexture() const { return &texture_; }

void TextureAtlas::Rebuild()
{
  std::vector<Image> images;
  std::vector<stbrp_rect> rects;

  for (const auto &file : image_files_) {
    const Image img = LoadImage(file.c_str());
    if (img.data == nullptr) { continue; }

    images.push_back(img);

    stbrp_rect r{};
    r.id = static_cast<int>(images.size() - 1);
    r.w = img.width;
    r.h = img.height;
    rects.push_back(r);
  }

  stbrp_context context;
  const int num_nodes = width_;
  std::vector<stbrp_node> nodes(num_nodes);
  stbrp_init_target(&context, width_, height_, nodes.data(), num_nodes);

  stbrp_pack_rects(&context, rects.data(), static_cast<int>(rects.size()));

  Image atlas_img = GenImageColor(width_, height_, BLANK);

  for (const auto &r : rects) {
    if (r.was_packed == 0) { continue; }
    const Image &img = images[r.id];
    const Rectangle src = { 0, 0, static_cast<float>(img.width), static_cast<float>(img.height) };
    const Rectangle dst = {
      static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(img.width), static_cast<float>(img.height)
    };
    ImageDraw(&atlas_img, img, src, dst, WHITE);
  }

  for (auto &img : images) { UnloadImage(img); }

  if (texture_.id != 0) { UnloadTexture(texture_); }
  texture_ = LoadTextureFromImage(atlas_img);
  SetTextureFilter(texture_, TEXTURE_FILTER_POINT);
  UnloadImage(atlas_img);

  regions_.clear();
  for (const auto &r : rects) {
    if (r.was_packed == 0) { continue; }

    AtlasRegion region;
    region.source = { .x = static_cast<float>(r.x),
      .y = static_cast<float>(r.y),
      .width = static_cast<float>(r.w),
      .height = static_cast<float>(r.h) };
    region.width = r.w;
    region.height = r.h;
    region.name = image_files_[r.id];
    regions_.emplace(region.name, region);
  }

  dirty_ = false;
}

void TextureAtlas::ClearAtlas()
{
  if (texture_.id != 0) {
    UnloadTexture(texture_);
    texture_ = {};
  }
  regions_.clear();
  image_files_.clear();
  dirty_ = false;
}
}// namespace chroma::client::render
