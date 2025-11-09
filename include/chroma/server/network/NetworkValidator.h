#pragma once

#include <enet.h>

#include "game_generated.h"

namespace chroma::server::network {

class NetworkValidator
{
public:
  static bool ValidatePeer(const ENetPeer *peer) { return peer != nullptr; }
  static bool ValidateEnvelope(const Game::Envelope *env) { return env != nullptr; }
};

}// namespace chroma::server::network
