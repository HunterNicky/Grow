#include "chroma/server/core/SessionManager.h"

#include <enet.h>
#include <uuid_v4.h>
#include <cstdint>

namespace chroma::server::core {

void SessionManager::AddSession(ENetPeer *peer, const UUIDv4::UUID &player_id)
{
  sessions_.emplace(peer, network::PlayerSession{ peer, player_id });
}

void SessionManager::RemoveSession(ENetPeer *peer) { sessions_.erase(peer); }

network::PlayerSession *SessionManager::GetSession(ENetPeer *peer)
{
  const auto it = sessions_.find(peer);
  if (it == sessions_.end()) { return nullptr; }
  return &it->second;
}

void SessionManager::UpdateLastProcessedInput(ENetPeer *peer, const uint32_t seq)
{
  auto *session = GetSession(peer);
  if (session != nullptr) { session->SetLastProcessedInput(seq); }
}

}// namespace chroma::server::core

