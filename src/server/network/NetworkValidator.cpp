#include "chroma/server/network/NetworkValidator.h"
#include "messages_generated.h"

#include <enet.h>

namespace chroma::server::network {

bool NetworkValidator::ValidatePeer(const ENetPeer *peer) { return peer != nullptr; }
bool NetworkValidator::ValidateEnvelope(const Game::Envelope *env) { return env != nullptr; }

}// namespace chroma::server::network