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
  const float default_width = 200;
  const float default_height = 50;
  const float padding = 20;
  const float start_x = bounds_.x + 50;
  float start_y = bounds_.y + 80;

  auto singleplayer_button = std::make_unique<client::ui::widget::Button>(
    "Play_Singleplayer", Rectangle{ start_x, start_y, default_width, default_height }, "Play SinglePlayer");
  start_y += default_height + padding;
  auto multiplayer_button = std::make_unique<client::ui::widget::Button>(
    "Play_Multiplayer", Rectangle{ start_x, start_y, default_width, default_height }, "Play Multiplayer");

  start_y += default_height + padding;
  auto options_button = std::make_unique<client::ui::widget::Button>(
    "Options", Rectangle{ start_x, start_y, default_width, default_height }, "Options");

  start_y += default_height + padding;  
  auto exit_button = std::make_unique<client::ui::widget::Button>(
    "Exit", Rectangle{ start_x, start_y, default_width, default_height }, "Exit");
  
  auto on_click_callback = [this](const std::string &button_id) {
    shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
    shared::event::EventBus::Dispatch(event);
  };

  singleplayer_button->setOnClick(on_click_callback);
  multiplayer_button->setOnClick(on_click_callback);
  options_button->setOnClick(on_click_callback);
  exit_button->setOnClick(on_click_callback);

  AddWidget(std::move(singleplayer_button));
  AddWidget(std::move(multiplayer_button));
  AddWidget(std::move(options_button));
  AddWidget(std::move(exit_button));
}
}// namespace chroma::client::ui::panel