#include "chroma/app/states/State.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/Subscription.h"

namespace chroma::app::states::menu {
class PauseState : public states::State
{
public:
  
  explicit PauseState();
  ~PauseState() override;
  PauseState(const PauseState &) = delete;
  PauseState &operator=(const PauseState &) = delete;
  PauseState(PauseState &&) = delete;
  PauseState &operator=(PauseState &&) = delete;

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
  void OnEvent(shared::event::Event &event) override;

private:
  shared::event::Subscription button_click_sub_;
};
}// namespace chroma::app::layer::state::menu