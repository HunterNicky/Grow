#include "chroma/client/factory/ShaderFactory.h"

#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/client/render/shader/shaders/CrtPass.h"
#include "chroma/client/render/shader/shaders/HealthPass.h"
#include "chroma/client/render/shader/shaders/ThresHoldPass.h"
#include "chroma/client/render/shader/shaders/BlurPass.h"
#include "chroma/client/render/shader/RenderPass.h"


namespace chroma::client::factory {

    using namespace render::shader;

    std::unique_ptr<RenderPass> ShaderPassFactory::Create(PassType type)
    {
        switch (type) {
        case PassType::CRT:
            return std::make_unique<shaders::CrtPass>();
        case PassType::HEALTH:
            return std::make_unique<shaders::HealthPass>();
        case PassType::THRESHOLD:
            return std::make_unique<shaders::ThresHoldPass>();
        case PassType::BLUR:
            return std::make_unique<shaders::BlurPass>();
        case PassType::BORDER:
            return std::make_unique<shaders::BorderPass>();
        default:
            return nullptr;
        }
    }
}