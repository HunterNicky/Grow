#pragma once 

#include "chroma/shared/events/Event.h"
#include "chroma/shared/core/player/Player.h"

#include <cstdint>
#include <map>
#include <memory>
#include <sys/types.h>

namespace chroma::app::states::network {
class PredictiveSyncSystem {
public:
    PredictiveSyncSystem() = default;
    ~PredictiveSyncSystem() = default;

    PredictiveSyncSystem(const PredictiveSyncSystem&) = default;
    PredictiveSyncSystem& operator=(const PredictiveSyncSystem&) = default;
    PredictiveSyncSystem(PredictiveSyncSystem&&) noexcept = default;
    PredictiveSyncSystem& operator=(PredictiveSyncSystem&&) noexcept = default;

    void ApplyEvents(shared::core::player::Player& player);
    void AddInputEventHistory(const shared::event::Event& event);
    void RemoveEventsAt(uint32_t seq);
    void ClearInputEventHistory();

    [[nodiscard]] uint32_t GetLastProcessedInputSeq() const;
    void SetLastProcessedInputSeq(uint32_t seq);

    [[nodiscard]] uint32_t GetNextSeq();
    void ResetSeqCounter();

private:
    std::map<uint32_t, std::shared_ptr<shared::event::Event>> input_event_history_;
    uint32_t last_processed_input_seq_ = 0;
    uint32_t seq_counter_ = 0;
};
} // namespace chroma::app::states::network