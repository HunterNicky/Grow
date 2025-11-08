#pragma once

#include <enet.h>
#include <uuid_v4.h>

namespace chroma::server::network {

class PlayerSession
{
public:
  PlayerSession() = default;
  PlayerSession(ENetPeer *peer, const UUIDv4::UUID &player_id) : peer_(peer), player_id_(player_id) {}

  [[nodiscard]] ENetPeer *GetPeer() const { return peer_; }
  [[nodiscard]] const UUIDv4::UUID &GetPlayerId() const { return player_id_; }
  [[nodiscard]] uint32_t GetLastProcessedInput() const { return last_processed_input_; }

  void SetPeer(ENetPeer *peer) { peer_ = peer; }
  void SetPlayerId(const UUIDv4::UUID &id) { player_id_ = id; }
  void SetLastProcessedInput(const uint32_t seq) { last_processed_input_ = seq; }

private:
  ENetPeer *peer_ = nullptr;
  UUIDv4::UUID player_id_;
  uint32_t last_processed_input_ = 0;
};

}// namespace chroma::server::network
