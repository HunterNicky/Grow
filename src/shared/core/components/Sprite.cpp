#include "chroma/shared/core/components/Sprite.h"
#include "chroma/shared/core/components/Component.h"

#include <raylib.h>
#include <string>

namespace chroma::shared::core::component {
Sprite::Sprite() : tint_(WHITE), flip_x_(false), flip_y_(false) { type_ = ComponentType::SPRITE_STATIC; }

void Sprite::SetSpriteId(const std::string &id) { sprite_id_ = id; }

const std::string &Sprite::GetSpriteId() const { return sprite_id_; }

void Sprite::SetTint(const Color tint) { tint_ = tint; }

Color Sprite::GetTint() const { return tint_; }

void Sprite::SetFlipX(const bool flip) { flip_x_ = flip; }

void Sprite::SetFlipY(const bool flip) { flip_y_ = flip; }

bool Sprite::GetFlipX() const { return flip_x_; }

bool Sprite::GetFlipY() const { return flip_y_; }
}// namespace chroma::shared::core::component
