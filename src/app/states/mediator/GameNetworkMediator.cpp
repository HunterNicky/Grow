#include <cstdint>
#include <uuid_v4.h>
#include <vector>

#include "chroma/app/states/mediator/GameNetworkMediator.h"
#include "chroma/app/states/GameState.h"
#include "chroma/app/states/network/NetworkState.h"
#include "chroma/shared/packet/PacketHandler.h"

namespace chroma::app::states {
    GameNetworkMediator::GameNetworkMediator(const std::shared_ptr<GameState>& game, const std::shared_ptr<NetworkState>& net)
    : game_state_(game), network_state_(net)
    {
    }

   void GameNetworkMediator::OnSnapshotReceived(const std::vector<uint8_t>& data) {
        auto state = game_state_.lock();
        if (!state) {
            return;
        }

        if(state->GetPlayerId() == UUIDv4::UUID{}) {
            state->SetPlayerId(shared::packet::PacketHandler::FlatBufferSnapshotGetUUID(data.data(), data.size()));
        }

        auto game_objects = state->GetGameObjects();
        chroma::shared::packet::PacketHandler::FlatBufferToGameObject(data.data(), data.size(), game_objects);
        state->SetGameObjects(game_objects);
    }
    void GameNetworkMediator::SendInput(const Game::InputMessage& input) {
        (void)input;
    }

    void GameNetworkMediator::SetGameState(const std::shared_ptr<GameState>& game) {
        game_state_ = game;
    }

    void GameNetworkMediator::SetNetworkState(const std::shared_ptr<NetworkState>& network) {
        network_state_ = network;
    }

    [[nodiscard]] std::shared_ptr<GameState> GameNetworkMediator::GetGameState() const {
        return game_state_.lock();
    }

    [[nodiscard]] std::shared_ptr<NetworkState> GameNetworkMediator::GetNetworkState() const {
        return network_state_.lock();
    }

    void GameNetworkMediator::RegisterPlayerId(const UUIDv4::UUID& player_id) {
        auto game_state = game_state_.lock();
        if (game_state) {
            game_state->SetPlayerId(player_id);
        }
    }
}