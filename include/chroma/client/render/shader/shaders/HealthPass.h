#pragma once

#include "chroma/client/render/shader/ShaderPass.h"
#include <raylib.h>

namespace chroma::client::render::shader::shaders {

class HealthPass : public ShaderPass {
public:
    HealthPass();
};
} // namespace chroma::client::render::shader::shaders