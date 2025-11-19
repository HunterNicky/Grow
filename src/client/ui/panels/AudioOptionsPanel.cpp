#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

#include "chroma/client/ui/panels/AudioOptionsPanel.h"
#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/widgets/Button.h"
#include "chroma/client/ui/widgets/SliderWidget.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"

namespace chroma::client::ui::panel {
AudioOptionsPanel::AudioOptionsPanel(Rectangle bounds) : Panel("AudioOptions", bounds) { CreateWidgets(); }

void AudioOptionsPanel::CreateWidgets()
{
  const float slider_width = 200.0F;
  const float slider_height = 40.0F;

  auto master_volume_slider = std::make_unique<client::ui::widget::SliderWidget>("GeneralVolume",
    Rectangle{ bounds_.x + 50, bounds_.y + 80, slider_width, slider_height },
    "General",
    0.0F,
    1.0F,
    0.75F);

  auto music_volume_slider = std::make_unique<client::ui::widget::SliderWidget>("MusicVolume",
    Rectangle{ bounds_.x + 50, bounds_.y + 140, slider_width, slider_height },
    "Music",
    0.0F,
    1.0F,
    0.50F);

  auto back_button = std::make_unique<client::ui::widget::Button>(
    "AudioBack", Rectangle{ bounds_.x + 50, bounds_.y + 200, slider_width, 50.0F }, "Back");

  auto on_click_callback = [this](const std::string &button_id) {
    shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
    shared::event::EventBus::Dispatch(event);
  };

  back_button->setOnClick(on_click_callback);

  AddWidget(std::move(master_volume_slider));
  AddWidget(std::move(music_volume_slider));
  AddWidget(std::move(back_button));
}

}// namespace chroma::client::ui::panel