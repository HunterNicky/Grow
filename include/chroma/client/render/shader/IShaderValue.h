    #pragma once

    #include <cstdint>
    #include <raylib.h>

    namespace chroma::client::render::shader {

    enum class UniformType : uint8_t
    { 
        FLOAT = 0,       
        VEC2,           
        VEC3,           
        VEC4,            
        INT,             
        IVEC2,           
        IVEC3,           
        IVEC4,           
        SAMPLER2D        
    };

    class IShaderValue {
    public:
        IShaderValue() = default;
        virtual ~IShaderValue() = default;

        IShaderValue(const IShaderValue&) = delete;
        IShaderValue& operator=(const IShaderValue&) = delete;
        IShaderValue(IShaderValue&&) = delete;
        IShaderValue& operator=(IShaderValue&&) = delete;

        [[nodiscard]] virtual int GetLocation() const = 0;
        [[nodiscard]] virtual UniformType GetType() const = 0;
        virtual void SetValue(const ::Shader& shader) const = 0;
    };

    } // chroma::client::render::shader