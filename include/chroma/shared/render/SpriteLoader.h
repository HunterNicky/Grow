#pragma once

#include "chroma/shared/core/components/SpriteAnimation.h"
#include <memory>

namespace chroma::shared::render {

class SpriteLoader
{
public:

    static void LoadSpriteAnimationFromFile(const std::shared_ptr<core::component::SpriteAnimation>& sprite_animation, const std::string& file_path);

private:
    static std::string GetBasePath(const std::string& path);
};

}// namespace chroma::shared::render