#pragma once

#include "chroma/shared/events/Event.h"
#include "chroma/app/database/DatabaseTypes.h" 

namespace chroma::shared::event {

enum class PlayerDataAction {
  Save = 0,
  Load = 1
};

class PlayerDataEvent : public Event
{
public:
  PlayerDataEvent(PlayerDataAction action, const app::database::PlayerData& data = {});
  
  [[nodiscard]] PlayerDataAction GetAction() const;
  [[nodiscard]] const app::database::PlayerData& GetData() const;
  
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  PlayerDataAction action_;
  app::database::PlayerData data_;
};

} // namespace chroma::shared::event