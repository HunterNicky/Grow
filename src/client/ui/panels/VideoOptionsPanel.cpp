#include <iostream>
#include <memory>
#include <raylib.h>
#include <string>
#include <utility>

#include "chroma/client/ui/panels/Panel.h"
#include "chroma/client/ui/panels/VideoOptionsPanel.h"
#include "chroma/client/ui/widgets/Button.h"
#include "chroma/client/ui/widgets/ToggleWidget.h"
#include "chroma/shared/events/Event.h"
#include "chroma/shared/events/EventBus.h"
#include "chroma/shared/events/ui/ButtonClickEvent.h"

namespace chroma::client::ui::panel {

VideoOptionsPanel::VideoOptionsPanel(Rectangle bounds) : Panel("VideoOptions", bounds) { CreateWidgets(); }

void VideoOptionsPanel::CreateWidgets()
{
  auto bloom_toggle = std::make_unique<client::ui::widget::ToggleWidget>(
    "Bloom", Rectangle{ bounds_.x + 50, bounds_.y + 80, 65, 30 }, "Bloom", true);

  auto scanlines_toggle = std::make_unique<client::ui::widget::ToggleWidget>(
    "Scanlines", Rectangle{ bounds_.x + 50, bounds_.y + 130, 65, 30 }, "Scanlines", false);

  auto back_button = std::make_unique<client::ui::widget::Button>(
    "VideoBack", Rectangle{ bounds_.x + 50, bounds_.y + 240, 200, 50 }, "Back");

  bloom_toggle->SetOnToggle([](bool is_on) { (void)is_on; std::cout << "bloom toggle\n"; });

  scanlines_toggle->SetOnToggle([](bool is_on) { (void)is_on; std::cout << "Scanlines toggle\n"; });

  auto on_click_callback = [this](const std::string &button_id) {
    shared::event::ui::ButtonClickEvent event(shared::event::Event::Type::ButtonClickEvent, button_id);
    shared::event::EventBus::Dispatch(event);
  };

  back_button->setOnClick(on_click_callback);

  AddWidget(std::move(bloom_toggle));
  AddWidget(std::move(scanlines_toggle));
  AddWidget(std::move(back_button));
}

}// namespace chroma::client::ui::panel