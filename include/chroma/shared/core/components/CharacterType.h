#pragma once

#include "chroma/shared/core/components/Component.h"
#include <cstdint>
#include <string>

namespace chroma::shared::core::component {
enum class CharacterType : uint8_t {
    NONE = 0,
    RANDI = 1,
    PRIMM = 2,
};

class CharacterTypeComponent : public Component {
public:
    explicit CharacterTypeComponent();

    [[nodiscard]] CharacterType GetCharacterType() const;
    void SetCharacterType(CharacterType type);

    [[nodiscard]] std::string GetPrefixCharacter() const;
private:
    CharacterType character_type_;
};

} // namespace chroma::shared::core::component