#pragma once

#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event {
class EventCatcher {
    public:
        static std::shared_ptr<event::Event> CatchEvent();
        static std::shared_ptr<event::Event> VerifyKeyEvent(int key_code);
};
}