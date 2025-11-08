#include "chroma/server/logic/ServerEventSystem.h"

#include "chroma/server/network/ServerPacketHandler.h"

namespace chroma::server::logic {

void ServerEventSystem::ProcessGameEvent(const ENetEvent &event,
  const long long elapsed,
  const Game::Event *evt,
  const Game::EventUnion event_union,
  ServerGameLogic &game_logic,
  core::SessionManager &sessions)
{
  switch (event_union) {
  case Game::EventUnion::InputEventMessage: {
    const auto input_message = network::ServerPacketHandler::EventToInputMessage(evt);
    if (input_message) {
      if (auto *session = sessions.GetSession(event.peer)) {
        game_logic.OnReceivedInputMessage(input_message, session->GetPlayerId());
        game_logic.Update(static_cast<float>(elapsed) / 1000.0F);
        sessions.UpdateLastProcessedInput(event.peer, input_message->GetSeq());
      }
    }
    break;
  }
  case Game::EventUnion::SoundEventMessage: {
    // No-op for now
    break;
  }
  default:
    break;
  }
}

}// namespace chroma::server::logic

