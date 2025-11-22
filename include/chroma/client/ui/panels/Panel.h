#pragma once

#include <memory>
#include <raylib.h>
#include <string>
#include <vector>

#include "chroma/client/ui/UIContext.h"
#include "chroma/client/ui/panels/PanelIdentifiers.h"
#include "chroma/client/ui/widgets/Widget.h"

namespace chroma::client::ui::panel {
class Panel
{
public:
  explicit Panel(const PanelID panel_id, Rectangle bounds);
  virtual ~Panel() = default;
  void OnUpdate(const float delta_time, const UIContext &context);
  void OnRender();
  void AddWidget(std::unique_ptr<widget::Widget> widget);
  void SetIsActive(const bool is_active);
  void SetIsVisible(const bool is_visible);
  [[nodiscard]] bool GetIsActive() const;
  [[nodiscard]] bool GetIsVisible() const;
  [[nodiscard]] PanelID GetID() const;

protected:
  Rectangle bounds_;

private:
  PanelID id_;
  std::vector<std::unique_ptr<widget::Widget>> widgets_;
  bool is_active_;
  bool is_visible_;
};
}// namespace chroma::client::ui::panel