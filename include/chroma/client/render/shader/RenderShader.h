    #pragma once 

    #include "chroma/client/render/shader/ShaderData.h"
    #include <vector>

    namespace chroma::client::render::shader {

    class RenderShader {

    public:
        ~RenderShader() = default;
        
        RenderShader(const RenderShader&) = delete;
        RenderShader& operator=(const RenderShader&) = delete;
        RenderShader(RenderShader&&) noexcept = default;
        RenderShader& operator=(RenderShader&&) noexcept = default;
        
        void AddShader(ShaderData&& data, std::string& name);
        void RemoveShader(const std::string& name);
        
        static RenderShader& GetInstance();

        void BeginRenderUniqueShader(const std::string& name);
        static void EndRenderUniqueShader(); 
        
        bool HasShaders() const;

        std::vector<ShaderData*> GetSortedShaders();        
    private:
        RenderShader() = default;
        std::unordered_map<std::string, ShaderData> shaders_;
    };

    }// chroma::client::render::shader