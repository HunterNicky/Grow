#pragma once

#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {

class Item : public Component {
public:
    explicit Item(std::string name);
    ~Item() override = default;

    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;
    Item(Item&&) noexcept = default;
    Item& operator=(Item&&) noexcept = default;

    [[nodiscard]] std::string GetName() const;
    void SetName(const std::string& name);

    [[nodiscard]] std::string GetDescription() const;
    void SetDescription(const std::string& description);

    [[nodiscard]] float GetWeight() const;
    void SetWeight(float weight);

private:
    std::string name_;
    std::string description_;
    float weight_;
};
} // namespace chroma::shared::core::component