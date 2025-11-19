#include "chroma/shared/core/components/Item.h"

namespace chroma::shared::core::component {
Item::Item(std::string name)
    : name_(std::move(name)), weight_(0.0F) {
    type_ = ComponentType::ITEM;
}
std::string Item::GetName() const {
    return name_;
}

void Item::SetName(const std::string& name) {
    name_ = name;
}

void Item::SetDescription(const std::string& description) {
    description_ = description;
}

std::string Item::GetDescription() const {
    return description_;
}

float Item::GetWeight() const {
    return weight_;
}

void Item::SetWeight(float weight) {
    weight_ = weight;
}

} // namespace chroma::shared::core::component