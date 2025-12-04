#pragma once

#include <cstdint>
#include <raylib.h>
#include <rlgl.h>

namespace chroma::client::render::shader {

class IShaderValue
{
public:
  IShaderValue() = default;
  virtual ~IShaderValue() = default;

  IShaderValue(const IShaderValue &) = delete;
  IShaderValue &operator=(const IShaderValue &) = delete;
  IShaderValue(IShaderValue &&) = delete;
  IShaderValue &operator=(IShaderValue &&) = delete;

  [[nodiscard]] virtual int GetLocation() const = 0;
  [[nodiscard]] virtual rlShaderUniformDataType GetType() const = 0;
  virtual void SetValue(const ::Shader &shader) const = 0;

  virtual void SetLocation(int location) = 0;
  virtual void SetType(rlShaderUniformDataType type) = 0;
};

}// namespace chroma::client::render::shader