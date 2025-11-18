#include "chroma/client/render/shader/shaders/HealthPass.h"

#include "chroma/shared/context/GameContext.h"

namespace chroma::client::render::shader::shaders {
    
    HealthPass::HealthPass() 
        : ShaderPass("resources/shaders/base.vs", "assets/shaders/health.fs") 
    {
        SetUniform("health", shader::UniformType::FLOAT, shared::context::GameContext::GetInstance().GetPlayerHealth());
        SetUniform("max_health", shader::UniformType::FLOAT, shared::context::GameContext::GetInstance().GetPlayerMaxHealth());
        SetUniform("time", shader::UniformType::FLOAT, shared::context::GameContext::GetInstance().GetDeltaTime());
    }
}