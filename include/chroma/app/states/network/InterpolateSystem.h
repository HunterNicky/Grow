#pragma once 

#include "chroma/shared/core/GameObject.h"

#include <unordered_map>

namespace chroma::app::states::network {
class InterpolateSystem {
public:
    InterpolateSystem() = default;
    ~InterpolateSystem() = default;

    InterpolateSystem(const InterpolateSystem&) = default;
    InterpolateSystem& operator=(const InterpolateSystem&) = default;
    InterpolateSystem(InterpolateSystem&&) noexcept = default;
    InterpolateSystem& operator=(InterpolateSystem&&) noexcept = default;

    void Interpolate(std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>>& game_objects);

private:

    std::unordered_map<UUIDv4::UUID, std::shared_ptr<chroma::shared::core::GameObject>> past_game_objects_;
};

} // namespace chroma::app::states::network