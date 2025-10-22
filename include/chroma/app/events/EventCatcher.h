#pragma once

#include "chroma/app/events/Event.h"

#include <memory>

namespace chroma::app::event {
class EventCatcher {
    public:
        static std::unique_ptr<event::Event> CatchEvent();
};
}