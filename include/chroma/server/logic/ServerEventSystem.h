#pragma once

#include <cstdint>
#include <enet.h>

#include "chroma/server/core/SessionManager.h"
#include "chroma/server/logic/ServerGameLogic.h"
#include "game_generated.h"

namespace chroma::server::logic {

class ServerEventSystem
{
public:
  static void ProcessGameEvent(const ENetEvent &event,
    long long elapsed,
    const Game::Event *evt,
    Game::EventUnion event_union,
    ServerGameLogic &game_logic,
    core::SessionManager &sessions);
};

}// namespace chroma::server::logic
