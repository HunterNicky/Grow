#pragma once

#include "chroma/client/render/shader/IShaderValue.h"
#include "chroma/client/render/shader/ShaderValue.h"
#include <raylib.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>

namespace chroma::client::render::shader {

class ShaderData {

public:
    explicit ShaderData() = default;

    ~ShaderData() = default;
    ShaderData(const ShaderData&) = delete;
    ShaderData& operator=(const ShaderData&) = delete;

    ShaderData(ShaderData&&) noexcept = default;
    ShaderData& operator=(ShaderData&&) noexcept = default;

    // ------------------------------------------------------------------
    // Carregar shader
    // ------------------------------------------------------------------
    void LoadShader(const std::string& vs, const std::string& fs)
    {
        shader_ = ::LoadShader(vs.c_str(), fs.c_str());

        if (shader_.id == 0) {
            throw std::runtime_error("Failed to load shader: " + fs);
        }
    }

    // ------------------------------------------------------------------
    // Descarregar shader
    // ------------------------------------------------------------------
    void Unload()
    {
        if (shader_.id != 0) {
            ::UnloadShader(shader_);
            shader_.id = 0;
        }
    }

    // ------------------------------------------------------------------
    // Acessar shader
    // ------------------------------------------------------------------
    [[nodiscard]] const ::Shader& GetShader() const {
        return shader_;
    }

    // ------------------------------------------------------------------
    // Recuperar localização de uniform
    // ------------------------------------------------------------------
    int GetLocation(const std::string& name)
    {
        return ::GetShaderLocation(shader_, name.c_str());
    }

    // ------------------------------------------------------------------
    // Registrar uniform (template completo no .h)
    // ------------------------------------------------------------------
    template<typename T>
    void SetUniform(const std::string& name, UniformType type, std::shared_ptr<T> value)
    {
        int loc = GetLocation(name);

        if (loc < 0) {
            std::cerr << "Warning: Uniform '" << name << "' not found.\n";
            return;
        }

        values_[name] = std::make_unique<ShaderValue<T>>(loc, type, value);
    }

    // ------------------------------------------------------------------
    // Prioridade
    // ------------------------------------------------------------------
    void SetPriority(int priority) {
        priority_ = priority;
    }

    [[nodiscard]] int GetPriority() const {
        return priority_;
    }

    // ------------------------------------------------------------------
    // Enviar todos os uniforms ao shader
    // ------------------------------------------------------------------
    void UploadAll() const
    {
        for (const auto& pair : values_) {
            pair.second->SetValue(shader_);
        }
    }

private:
    ::Shader shader_{};
    std::unordered_map<std::string, std::unique_ptr<IShaderValue>> values_;
    int priority_ = 0;
};

} // namespace chroma::client::render::shader
