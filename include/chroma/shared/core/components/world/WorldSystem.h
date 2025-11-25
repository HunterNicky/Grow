#pragma once
#include "chroma/shared/core/components/Component.h"
#include "chroma/shared/core/world/WorldSystem.h"

namespace chroma::shared::core::component {
class WorldSystem final : public Component, public std::enable_shared_from_this<WorldSystem>
{
public:
  WorldSystem();
  ~WorldSystem() override = default;

  void Initialize(const std::string &path);

  [[nodiscard]] std::vector<world::RenderTile> GetRenderTile() const;

  void Update(float delta_time) override;
  void Render() override;

private:
  std::unique_ptr<world::WorldSystem> world_system_;
};
}// namespace chroma::shared::core