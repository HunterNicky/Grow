#pragma once

#include <cstdint>
#include <string>

namespace chroma::shared::packet {
class SoundEventMessage {
public:
  SoundEventMessage() = default;
  SoundEventMessage(uint32_t sequence, float delta_time, std::string sound_id, float volume, float pitch)
    : seq_(sequence), dt_(delta_time), sound_id_(std::move(sound_id)), volume_(volume), pitch_(pitch) {}

  SoundEventMessage(const SoundEventMessage &) = default;
  SoundEventMessage(SoundEventMessage &&) = default;
  SoundEventMessage &operator=(const SoundEventMessage &) = default;
  SoundEventMessage &operator=(SoundEventMessage &&) = default;
  ~SoundEventMessage() = default;

  [[nodiscard]] uint32_t GetSeq() const { return seq_; }
  [[nodiscard]] float GetDeltaTime() const { return dt_; }
  [[nodiscard]] const std::string &GetSoundId() const { return sound_id_; }
  [[nodiscard]] float GetVolume() const { return volume_; }
  [[nodiscard]] float GetPitch() const { return pitch_; }
  [[nodiscard]] const std::string &GetSourceId() const { return source_id_; }

  void SetSeq(uint32_t seq) { seq_ = seq; }
  void SetDeltaTime(float dt) { dt_ = dt; }
  void SetSoundId(std::string id) { sound_id_ = std::move(id); }
  void SetVolume(float v) { volume_ = v; }
  void SetPitch(float p) { pitch_ = p; }
  void SetSourceId(std::string id) { source_id_ = std::move(id); }

private:
  uint32_t seq_{};
  float dt_{};
  std::string sound_id_{};
  float volume_{};
  float pitch_{};
  std::string source_id_{};
};
} // namespace chroma::shared::packet
