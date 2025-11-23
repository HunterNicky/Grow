#include "chroma/shared/render/SpriteLoader.h"
#include "chroma/shared/core/components/SpriteAnimation.h"

#include <fstream>
#include <string>
#include <memory>
#include <cstddef>
#include <nlohmann/json.hpp>

namespace chroma::shared::render {

std::string SpriteLoader::GetBasePath(const std::string &path)
{
  const size_t slash = path.find_last_of("/\\");
  if (slash == std::string::npos) { return ""; }
  return path.substr(0, slash + 1);
}

void SpriteLoader::LoadSpriteAnimationFromFile(
  const std::shared_ptr<core::component::SpriteAnimation> &sprite_animation,
  const std::string &file_path)
{
  std::ifstream file(file_path);
  nlohmann::json j;
  file >> j;

  const std::string base_path = GetBasePath(file_path);
  const std::string sprite_sheet_path = base_path + j["sprite_sheet"].get<std::string>();

  auto &animations = j["animations"];

  for (const auto &[anim_name, anim_data] : animations.items()) {

    core::component::SpriteAnimationDesc desc;
    desc.name = anim_name;
    desc.loop = anim_data["loop"].get<bool>();

    for (auto &frame : anim_data["frames"]) {
      core::component::SpriteAnimFrame f;

      f.sprite_id = sprite_sheet_path;
      f.duration_ticks = frame["duration_ticks"].get<int>();

      f.subregion.x = frame["x"].get<float>();
      f.subregion.y = frame["y"].get<float>();
      f.subregion.width = frame["width"].get<float>();
      f.subregion.height = frame["height"].get<float>();

      desc.frames.push_back(f);
    }

    sprite_animation->LoadAnimation(anim_name, desc);
  }
}

}// namespace chroma::shared::render
