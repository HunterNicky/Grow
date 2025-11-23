#pragma once

#include <enet.h>
#include <uuid_v4.h>

namespace chroma::server::network {

class PlayerSession
{
public:
  PlayerSession() = default;
  PlayerSession(ENetPeer *peer, const UUIDv4::UUID &player_id);

  [[nodiscard]] ENetPeer *GetPeer() const;
  [[nodiscard]] const UUIDv4::UUID &GetPlayerId() const;
  [[nodiscard]] uint32_t GetLastProcessedInput() const;

  void SetPeer(ENetPeer *peer);
  void SetPlayerId(const UUIDv4::UUID &id);
  void SetLastProcessedInput(const uint32_t seq);

private:
  ENetPeer *peer_ = nullptr;
  UUIDv4::UUID player_id_;
  uint32_t last_processed_input_ = 0;
};

}// namespace chroma::server::network
