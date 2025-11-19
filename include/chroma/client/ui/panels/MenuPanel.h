#include "chroma/client/ui/panels/Panel.h"

namespace chroma::client::ui::panel {
class MenuPanel : public Panel
{
public:
  explicit MenuPanel(Rectangle bounds);

private:
  void CreateWidgets();
};
}// namespace chroma::client::ui::panel