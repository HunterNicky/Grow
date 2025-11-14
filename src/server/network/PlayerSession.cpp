#include "chroma/server/network/PlayerSession.h"


#include <enet.h>
#include <uuid_v4.h>

namespace chroma::server::network {

    PlayerSession::PlayerSession(ENetPeer *peer, const UUIDv4::UUID &player_id) : peer_(peer), player_id_(player_id) {}

    ENetPeer* PlayerSession::GetPeer() const { return peer_; }
    const UUIDv4::UUID& PlayerSession::GetPlayerId() const { return player_id_; }
    uint32_t PlayerSession::GetLastProcessedInput() const { return last_processed_input_; }

    void PlayerSession::SetPeer(ENetPeer *peer) { peer_ = peer; }
    void PlayerSession::SetPlayerId(const UUIDv4::UUID &id) { player_id_ = id; }
    void PlayerSession::SetLastProcessedInput(const uint32_t seq) { last_processed_input_ = seq; }
}