#include "chroma/shared/events/Subscription.h"
#include "chroma/shared/events/EventDispatcher.h"
#include <utility>

namespace chroma::shared::event {
Subscription::Subscription(SubscriptionInfo info) : info_(std::move(info)) {}

Subscription::~Subscription()
{
  if (info_.id_ == 0) { return; }
  if (auto dispatcher = info_.dispatcher_.lock()) { dispatcher->Unsubscribe(info_); }
}

Subscription::Subscription(Subscription &&other) noexcept : info_(std::move(other.info_)) { other.info_.id_ = 0; }

Subscription &Subscription::operator=(Subscription &&other) noexcept
{
  if (this == &other) { return *this; }

  info_ = std::move(other.info_);
  other.info_.id_ = 0;

  return *this;
}

const SubscriptionInfo &Subscription::GetSubscriptionInfo() const { return info_; }

}// namespace chroma::shared::event