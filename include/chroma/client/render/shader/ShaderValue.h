#pragma once

#include <raylib.h>
#include <memory>

#include "chroma/client/render/shader/IShaderValue.h"

namespace chroma::client::render::shader {

template<typename T>
class ShaderValue : public IShaderValue {
public:
    ShaderValue() = default;

    ShaderValue(int location, UniformType type, const std::shared_ptr<T>& data)
        :  location_(location), type_(type), data_(data) {}

    ~ShaderValue() override = default;

    ShaderValue(const ShaderValue&) = delete;
    ShaderValue& operator=(const ShaderValue&) = delete;
    ShaderValue(ShaderValue&&) noexcept = default;
    ShaderValue& operator=(ShaderValue&&) noexcept = default;

    void SetLocation(int location) override {
        location_ = location;
    }

    [[nodiscard]] int GetLocation() const override {
        return location_;
    }

    void SetType(UniformType type) override{
        type_ = type;
    }

    [[nodiscard]] UniformType GetType() const override {
        return type_;
    }

    void SetData(const std::shared_ptr<T>& data) {
        data_ = data;
    }

    [[nodiscard]] std::shared_ptr<T> GetDataPtr() const {
        return data_.lock();
    }

    void SetValue(const ::Shader& shader) const override {
        auto ptr = data_.lock();
        if (!ptr) {return;}
        
        SetShaderValue(shader, location_, ptr.get(), static_cast<int>(type_));
    }

private:
    int location_ = -1;
    UniformType type_ = UniformType::FLOAT;
    std::weak_ptr<T> data_;
};

} // namespace chroma::client::render::shader
