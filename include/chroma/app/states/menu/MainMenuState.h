#include "chroma/app/states/State.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app::states::menu {
class MainMenuState : public states::State
{
public:
  explicit MainMenuState();
  ~MainMenuState() override;

  MainMenuState(const MainMenuState &) = delete;
  MainMenuState &operator=(const MainMenuState &) = delete;
  MainMenuState(MainMenuState &&) = delete;
  MainMenuState &operator=(MainMenuState &&) = delete;

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
  void OnEvent(shared::event::Event &event) override;

private:
  shared::event::Subscription button_click_sub_;
};
}// namespace chroma::app::layer::state::menu