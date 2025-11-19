#include <memory>
#include <raylib.h>
#include <utility>
#include <string>

#include "chroma/client/ui/panels/MenuPanel.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/widgets/Button.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/EventBus.h"

namespace chroma::client::ui::panel {
MenuPanel::MenuPanel(Rectangle bounds) : Panel("Menu", bounds) { CreateWidgets(); }

void MenuPanel::CreateWidgets()
{
  auto play_button = std::make_unique<client::ui::widget::Button>(
    "Play", Rectangle{ bounds_.x + 50, bounds_.y + 80, 200, 50 }, "Play");

  auto options_button = std::make_unique<client::ui::widget::Button>(
    "Options", Rectangle{ bounds_.x + 50, bounds_.y + 160, 200, 50 }, "Options");

  auto exit_button = std::make_unique<client::ui::widget::Button>(
    "Exit", Rectangle{ bounds_.x + 50, bounds_.y + 240, 200, 50 }, "Exit");

  auto on_click_callback = [this](const std::string &button_id) {
    shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
    shared::event::EventBus::Dispatch(event);
  };

  play_button->setOnClick(on_click_callback);
  options_button->setOnClick(on_click_callback);
  exit_button->setOnClick(on_click_callback);

  AddWidget(std::move(play_button));
  AddWidget(std::move(options_button));
  AddWidget(std::move(exit_button));
}
}// namespace chroma::client::ui::panel