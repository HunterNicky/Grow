#include <memory>
#include <string>

#include "chroma/app/states/State.h"
#include "chroma/client/ui/UIManager.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app::layer::state::menu {
class OptionsMenuState : public states::State
{
public:
  explicit OptionsMenuState();
  ~OptionsMenuState() override;

  virtual void OnUpdate(const float delta_time) override;
  virtual void OnRender() override;
  virtual void OnEvent(shared::event::Event &event) override;

private:
  shared::event::Subscription button_click_sub_;
};
}// namespace chroma::app::layer::state::menu