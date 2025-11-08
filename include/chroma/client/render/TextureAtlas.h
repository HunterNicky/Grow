#pragma once

#include <optional>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace chroma::client::render {
struct AtlasRegion
{
  Rectangle source;
  int width;
  int height;
  std::string name;
};

class TextureAtlas
{
public:
  explicit TextureAtlas(int width = 4096, int height = 4096);
  TextureAtlas(const TextureAtlas &) = default;
  TextureAtlas(TextureAtlas &&) = delete;
  TextureAtlas &operator=(const TextureAtlas &) = default;
  TextureAtlas &operator=(TextureAtlas &&) = delete;
  ~TextureAtlas();

  void AddImage(const std::string &filepath);
  [[nodiscard]] std::optional<const AtlasRegion *> GetRegion(const std::string &id);
  [[nodiscard]] const Texture *GetTexture() const;

private:
  void Rebuild();
  void ClearAtlas();

  int width_, height_;
  bool dirty_;

  Texture2D texture_;

  std::unordered_map<std::string, AtlasRegion> regions_;
  std::vector<std::string> image_files_;
};
}// namespace chroma::client::render
