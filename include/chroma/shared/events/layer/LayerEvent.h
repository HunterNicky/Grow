#pragma once

#include "chroma/app/layers/LayerIdentifiers.h"
#include "chroma/shared/events/Event.h"

#include <memory>

namespace chroma::shared::event::layer {

enum class Action { Pop = 0, Push = 1 };

class LayerEvent : public Event
{
public:
  explicit LayerEvent(const Action action);
  LayerEvent(const Action action, const app::layer::LayerID layer_id);

  [[nodiscard]] app::layer::LayerID GetLayerId() const;
  [[nodiscard]] Action GetAction() const;
  [[nodiscard]] std::shared_ptr<Event> Clone() const override;
  [[nodiscard]] static Type GetStaticType();

private:
  Action action_;
  app::layer::LayerID layer_id_;
};
}// namespace chroma::shared::event::layer