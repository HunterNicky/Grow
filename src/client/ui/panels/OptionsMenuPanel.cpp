#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

#include "chroma/client/ui/panels/OptionsMenuPanel.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/widgets/Button.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"

namespace chroma::client::ui::panel {

OptionsMenuPanel::OptionsMenuPanel(Rectangle bounds) : Panel("Options", bounds) { CreateWidgets(); }

void OptionsMenuPanel::CreateWidgets()
{
  auto video_button = std::make_unique<client::ui::widget::Button>(
    "Video", Rectangle{ bounds_.x + 50, bounds_.y + 80, 200, 50 }, "Video");

  auto audio_button = std::make_unique<client::ui::widget::Button>(
    "Audio", Rectangle{ bounds_.x + 50, bounds_.y + 160, 200, 50 }, "Audio");

  auto back_button = std::make_unique<client::ui::widget::Button>(
    "Back", Rectangle{ bounds_.x + 50, bounds_.y + 240, 200, 50 }, "Back");

  auto on_click_callback = [this](const std::string &button_id) {
    shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
    shared::event::EventBus::Dispatch(event);
  };

  video_button->setOnClick(on_click_callback);
  audio_button->setOnClick(on_click_callback);
  back_button->setOnClick(on_click_callback);

  AddWidget(std::move(video_button));
  AddWidget(std::move(audio_button));
  AddWidget(std::move(back_button));
}

}// namespace chroma::client::ui::panel