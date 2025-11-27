#pragma once
#include <cstdint>
#include <raylib.h>
#include <string>

namespace chroma::client::render::shader {

enum class PassType : uint8_t {
  NONE = 0,
  BLUR = 1,
  CRT = 2,
  BORDER = 3,
  DERIVATIVEX = 4,
  DERIVATIVEY = 5,
  GRAYSCALE = 6,
  HEALTH = 7,
  THRESHOLD = 8
};

class RenderPass
{
public:
  virtual ~RenderPass() = default;

  RenderPass(std::string vs_path, std::string fs_path) : vs_path_(std::move(vs_path)), fs_path_(std::move(fs_path)) {}
  RenderPass(const RenderPass &) = delete;
  RenderPass &operator=(const RenderPass &) = delete;
  RenderPass(RenderPass &&) = delete;
  RenderPass &operator=(RenderPass &&) = delete;

  virtual void Setup() = 0;
  virtual void Execute(RenderTexture2D &src, RenderTexture2D &dst) = 0;

  [[nodiscard]] PassType GetPassType() const { return type_; };
  void SetPassType(PassType type) { type_ = type; };

  [[nodiscard]] bool IsActive() const { return active_; }
  void SetActive(bool active) { active_ = active; }

protected:
  std::string vs_path_;
  std::string fs_path_;
  PassType type_{ PassType::NONE };
  bool active_{ true };
};

}// namespace chroma::client::render::shader
