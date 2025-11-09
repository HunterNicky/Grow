#pragma once

#include <unordered_map>

#include <enet.h>
#include <uuid_v4.h>

#include "chroma/server/network/PlayerSession.h"

namespace chroma::server::core {

class SessionManager
{
public:
  void AddSession(ENetPeer *peer, const UUIDv4::UUID &player_id);
  void RemoveSession(ENetPeer *peer);

  network::PlayerSession *GetSession(ENetPeer *peer);

  [[nodiscard]] const std::unordered_map<ENetPeer *, network::PlayerSession> &GetAll() const { return sessions_; }
  std::unordered_map<ENetPeer *, network::PlayerSession> &GetAllMutable() { return sessions_; }

  void UpdateLastProcessedInput(ENetPeer *peer, uint32_t seq);

private:
  std::unordered_map<ENetPeer *, network::PlayerSession> sessions_;
};

}// namespace chroma::server::core
