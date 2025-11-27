#pragma once

#include "chroma/client/render/shader/RenderPass.h"
#include <memory>

namespace chroma::client::factory {

class ShaderPassFactory {
public:
    static std::unique_ptr<render::shader::RenderPass> Create(render::shader::PassType type);
};

} // namespace chroma::shared::factory
