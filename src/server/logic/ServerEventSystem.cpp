#include "chroma/server/logic/ServerEventSystem.h"

#include "chroma/server/core/SessionManager.h"
#include "chroma/server/logic/ServerGameLogic.h"
#include "chroma/server/network/ServerPacketHandler.h"
#include "events_generated.h"

#include <enet.h>

namespace chroma::server::logic {
void ServerEventSystem::ProcessGameEvent(const ENetEvent &event,
  const long long elapsed,
  const Game::Event *evt,
  const Game::EventUnion event_union,
  ServerGameLogic &game_logic,
  core::SessionManager &sessions)
{
  (void)elapsed;
  switch (event_union) {
  case Game::EventUnion::InputEventMessage: {
    const auto input_message = network::ServerPacketHandler::EventToInputMessage(evt);
    if (input_message) {
      if (auto *session = sessions.GetSession(event.peer)) {
        game_logic.OnReceivedInputMessage(input_message, session->GetPlayerId());
        sessions.UpdateLastProcessedInput(event.peer, input_message->GetSeq());
      }
    }
    break;
  }
  case Game::EventUnion::SoundEventMessage: {
    // No-op for now
    break;
  }
  case Game::EventUnion::ProjectileEventMessage: {
    const auto projectile_message = network::ServerPacketHandler::EventToProjectileMessage(evt);
    if (projectile_message) { game_logic.OnReceivedProjectileMessage(projectile_message); }
    break;
  }
  default:
    break;
  }
}

}// namespace chroma::server::logic
