#include "chroma/shared/core/components/CharacterType.h"

namespace chroma::shared::core::component {
CharacterTypeComponent::CharacterTypeComponent()
    : character_type_(CharacterType::NONE)
{
    type_ = ComponentType::CHARACTER_TYPE;
}

CharacterType CharacterTypeComponent::GetCharacterType() const {
    return character_type_;
}

void CharacterTypeComponent::SetCharacterType(CharacterType type) {
    character_type_ = type;
}

std::string CharacterTypeComponent::GetPrefixCharacter() const {
    switch (character_type_) {
        case CharacterType::RANDI:
            return "randi";
        case CharacterType::PRIMM:
            return "primm";
        case CharacterType::NONE:
        default:
            return "none";
    }
}
} // namespace chroma::shared::core::component