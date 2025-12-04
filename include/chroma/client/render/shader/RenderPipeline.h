#pragma once

#include "RenderPass.h"

#include <algorithm>
#include <memory>
#include <raylib.h>
#include <vector>

namespace chroma::client::render::shader {

class RenderPipeline
{
public:
  RenderPipeline() = default;
  ~RenderPipeline();
  RenderPipeline(const RenderPipeline &) = delete;
  RenderPipeline &operator=(const RenderPipeline &) = delete;
  RenderPipeline(RenderPipeline &&) noexcept = default;
  RenderPipeline &operator=(RenderPipeline &&) noexcept = default;

  void Initialize(int width, int height);

  void AddPassFront(std::unique_ptr<RenderPass> pass);
  void AddPassBack(std::unique_ptr<RenderPass> pass);

  void Setup() const;

  RenderTexture2D &Execute(RenderTexture2D &original_scene);

  [[nodiscard]] RenderPass *GetPassByType(PassType type) const;
  void RemovePassByType(PassType type);
  void ReloadPassByType(PassType type) const;
  void MovePass(PassType type, int direction);

  [[nodiscard]] std::vector<PassType> GetActivePassTypes() const;

  template<typename T> T *GetPass() const
  {
    auto it = std::find_if(passes_.begin(), passes_.end(), [](const std::unique_ptr<RenderPass> &p) {
      return dynamic_cast<T *>(p.get()) != nullptr;
    });

    return it != passes_.end() ? dynamic_cast<T *>(it->get()) : nullptr;
  }

private:
  std::vector<std::unique_ptr<RenderPass>> passes_;

  RenderTexture2D ping_{};
  RenderTexture2D pong_{};
};

}// namespace chroma::client::render::shader