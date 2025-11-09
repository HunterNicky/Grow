#include "chroma/app/states/network/PredictiveSyncSystem.h"
#include "chroma/shared/core/player/Player.h"
#include "chroma/shared/events/Event.h"

#include <cstdint>
#include <memory>

namespace chroma::app::states::network {
void PredictiveSyncSystem::ApplyEvents(const std::shared_ptr<shared::core::player::Player> &player) const
{
  for (const auto &[seq, event] : input_event_history_) { player->HandleEvent(*event); }
}

void PredictiveSyncSystem::AddInputEventHistory(const shared::event::Event &event)
{
  input_event_history_[GetNextSeq()] = event.Clone();
}

void PredictiveSyncSystem::RemoveEventsAt(uint32_t seq)
{
  for (auto it = input_event_history_.begin(); it != input_event_history_.end();) {
    if (it->first <= seq) {
      it = input_event_history_.erase(it);
    } else {
      ++it;
    }
  }
}

void PredictiveSyncSystem::ClearInputEventHistory() { input_event_history_.clear(); }

uint32_t PredictiveSyncSystem::GetLastProcessedInputSeq() const { return last_processed_input_seq_; }

void PredictiveSyncSystem::SetLastProcessedInputSeq(uint32_t seq) { last_processed_input_seq_ = seq; }

uint32_t PredictiveSyncSystem::GetNextSeq() { return seq_counter_++; }

uint32_t PredictiveSyncSystem::GetSeqCounter() const { return seq_counter_; }

void PredictiveSyncSystem::ResetSeqCounter() { seq_counter_ = 0; }

}// namespace chroma::app::states::network