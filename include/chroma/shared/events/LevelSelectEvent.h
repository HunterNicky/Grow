#pragma once

#include "chroma/shared/events/Event.h"

#include <string>

namespace chroma::shared::event {

class LevelSelectEvent : public Event
{
public:
  explicit LevelSelectEvent(std::string level_id);

  [[nodiscard]] const std::string &GetLevelId() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  std::string level_id_;
};
}// namespace chroma::shared::event
