#pragma once

#include <memory>
#include <raylib.h>

#include "chroma/client/render/shader/IShaderValue.h"

namespace chroma::client::render::shader {

template<typename T> class ShaderValue : public IShaderValue
{
public:
  ShaderValue() = default;

  ShaderValue(int location, rlShaderUniformDataType type, const std::shared_ptr<T> &data)
    : location_(location), type_(type), data_(data)
  {}

  ~ShaderValue() override = default;

  ShaderValue(const ShaderValue &) = delete;
  ShaderValue &operator=(const ShaderValue &) = delete;
  ShaderValue(ShaderValue &&) noexcept = default;
  ShaderValue &operator=(ShaderValue &&) noexcept = default;

  void SetLocation(int location) override { location_ = location; }

  [[nodiscard]] int GetLocation() const override { return location_; }

  void SetType(rlShaderUniformDataType type) override { type_ = type; }

  [[nodiscard]] rlShaderUniformDataType GetType() const override { return type_; }

  void SetData(const std::shared_ptr<T> &data) { data_ = data; }

  [[nodiscard]] std::shared_ptr<T> GetDataPtr() const { return data_.lock(); }

  void SetValue(const ::Shader &shader) const override
  {
    if (!data_) { return; }

    SetShaderValue(shader, location_, data_.get(), type_);
  }

private:
  int location_ = -1;
  rlShaderUniformDataType type_ = rlShaderUniformDataType::RL_SHADER_UNIFORM_FLOAT;
  std::shared_ptr<T> data_;
};

}// namespace chroma::client::render::shader
