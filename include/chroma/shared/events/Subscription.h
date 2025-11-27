#pragma once

#include "chroma/shared/events/Event.h"

#include <cstdint>

namespace chroma::shared::event {
struct SubscriptionInfo
{
  Event::Type event_type_ = Event::Type::None;
  uint64_t id_ = 0;
};

class Subscription
{
public:
  Subscription() = default;
  explicit Subscription(const SubscriptionInfo &info);
  ~Subscription();

  Subscription(const Subscription &) = delete;
  Subscription &operator=(const Subscription &) = delete;

  Subscription(Subscription &&other) noexcept;
  Subscription &operator=(Subscription &&other) noexcept;

  const SubscriptionInfo &GetSubscriptionInfo() const;

private:
  SubscriptionInfo info_;
};
}// namespace chroma::shared::event