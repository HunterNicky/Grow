#pragma once

#include "chroma/shared/events/EventDispatcher.h"

#include <memory>

namespace chroma::shared::event {
class EventCatcher
{
public:
  static void CatchEvent();
};
}// namespace chroma::shared::event