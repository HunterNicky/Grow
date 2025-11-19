#include "chroma/client/ui/panels/Panel.h"

namespace chroma::client::ui::panel {
class OptionsMenuPanel : public Panel
{
public:
  explicit OptionsMenuPanel(Rectangle bounds);

private:
  void CreateWidgets();
};
}// namespace chroma::client::ui::panel