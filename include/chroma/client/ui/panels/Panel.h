#pragma once

#include <memory>
#include <raylib.h>
#include <vector>
#include <string>
#include <string_view>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::panel {
class Panel
{
public:
  explicit Panel(std::string id, Rectangle bounds);
  virtual ~Panel() = default;
  void OnUpdate(const float delta_time, const UIContext &context);
  void OnRender();
  void AddWidget(std::unique_ptr<widget::Widget> widget);
  void SetIsActive(const bool is_active);
  void SetIsVisible(const bool is_visible);
  [[nodiscard]] bool GetIsActive() const;
  [[nodiscard]] bool GetIsVisible() const;
  [[nodiscard]] std::string_view GetID() const;

protected:
  Rectangle bounds_;

private:
  std::string id_;
  std::vector<std::unique_ptr<widget::Widget>> widgets_;
  bool is_active_;
  bool is_visible_;
};
}// namespace chroma::client::ui::panel