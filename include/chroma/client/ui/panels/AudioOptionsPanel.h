#pragma once

#include <raylib.h>
#include <string>

#include "chroma/client/ui/panels/Panel.h"

namespace chroma::client::ui::panel {

class AudioOptionsPanel : public Panel
{
public:
  explicit AudioOptionsPanel(Rectangle bounds);
  ~AudioOptionsPanel() override = default;

private:
  void CreateWidgets();
};

}// namespace chroma::client::ui::panel