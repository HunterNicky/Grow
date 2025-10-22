#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event {
class EventCatcher {
    public:
        static std::unique_ptr<event::Event> CatchEvent();
};
}