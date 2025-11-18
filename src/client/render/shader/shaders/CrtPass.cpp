#include "chroma/client/render/shader/shaders/CrtPass.h"

namespace chroma::client::render::shader::shaders {

CrtPass::CrtPass() 
    : ShaderPass("resources/shaders/base.vs", "assets/shaders/crt.fs") 
{
}
}