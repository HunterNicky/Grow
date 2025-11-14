#pragma once

#include <enet.h>

#include "messages_generated.h"

namespace chroma::server::network {

class NetworkValidator
{
public:
  static bool ValidatePeer(const ENetPeer *peer);
  static bool ValidateEnvelope(const Game::Envelope *env);
};

}// namespace chroma::server::network
