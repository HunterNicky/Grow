#include "chroma/client/ui/panels/Panel.h"

namespace chroma::client::ui::panel {
class VideoOptionsPanel : public Panel
{
public:
  explicit VideoOptionsPanel(Rectangle bounds);

private:
  void CreateWidgets();
};
}// namespace chroma::client::ui::panel