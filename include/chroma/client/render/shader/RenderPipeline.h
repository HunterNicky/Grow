#pragma once

#include "RenderPass.h"
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

  void Setup();

  RenderTexture2D &Execute(RenderTexture2D &original_scene);

  [[nodiscard]] RenderPass* GetPassByType(PassType type) const;
  void RemovePassByType(PassType type);

  template<typename T>
  T* GetPass() const {
      for (const auto &p : passes_) {
          if (auto casted = dynamic_cast<T*>(p.get()))
          {
            return casted;
          }
      }
      return nullptr;
  }

private:
  std::vector<std::unique_ptr<RenderPass>> passes_;

  RenderTexture2D ping_{};
  RenderTexture2D pong_{};
};

}// namespace chroma::client::render::shader