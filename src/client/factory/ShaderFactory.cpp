#include "chroma/client/factory/ShaderFactory.h"
#include "chroma/client/render/shader/RenderPass.h"
#include "chroma/client/render/shader/shaders/BlurPass.h"
#include "chroma/client/render/shader/shaders/BorderColorPass.h"
#include "chroma/client/render/shader/shaders/BorderPass.h"
#include "chroma/client/render/shader/shaders/CrtPass.h"
#include "chroma/client/render/shader/shaders/GrayPass.h"
#include "chroma/client/render/shader/shaders/HealthPass.h"
#include "chroma/client/render/shader/shaders/ThresholdPass.h"

#include <chroma/client/render/shader/shaders/DerivativeXPass.h>
#include <chroma/client/render/shader/shaders/DerivativeYPass.h>
#include <memory>

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
    return std::make_unique<shaders::ThresholdPass>();
  case PassType::BLUR:
    return std::make_unique<shaders::BlurPass>();
  case PassType::BORDER:
    return std::make_unique<shaders::BorderPass>();
  case PassType::DERIVATIVEX:
    return std::make_unique<shaders::DerivativeXPass>();
  case PassType::DERIVATIVEY:
    return std::make_unique<shaders::DerivativeYPass>();
  case PassType::GRAYSCALE:
    return std::make_unique<shaders::GrayPass>();
  case PassType::BORDERCOLOR:
    return std::make_unique<shaders::BorderColorPass>();
  default:
    return nullptr;
  }
}
}// namespace chroma::client::factory