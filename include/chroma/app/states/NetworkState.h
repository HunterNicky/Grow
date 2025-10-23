#pragma once

#include <cstdint>
#include <enet.h>
#include <memory>
#include <vector>

#include "chroma/shared/core/GameObject.h"
#include "chroma/app/states/State.h"
#include "chroma/app/states/mediator/GameNetworkMediator.h"

namespace chroma::app::states {
class NetworkState : public State
{
public:
    NetworkState();
    explicit NetworkState(std::shared_ptr<chroma::app::states::GameNetworkMediator> game_mediator);
    ~NetworkState() override;

    NetworkState(const NetworkState&) = delete;
    NetworkState& operator=(const NetworkState&) = delete;
    NetworkState(NetworkState&&) = delete;
    NetworkState& operator=(NetworkState&&) = delete;
    
    void OnUpdate(float delta_time) override;
    void OnEvent(shared::event::Event& event) override;

private:

    std::unique_ptr<ENetHost, decltype(&enet_host_destroy)> client_;
    std::unique_ptr<ENetPeer, decltype(&enet_peer_reset)> server_peer_;
    ENetAddress server_address_;
    ENetEvent event_;
    uint32_t seq_num_ = 0;
    bool connected_ = false;

    std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_past_;
    std::vector<std::shared_ptr<chroma::shared::core::GameObject>> game_objects_present_;
    std::shared_ptr<chroma::app::states::GameNetworkMediator> game_mediator_;


    static void PeerDeleter(ENetPeer* peer);
    bool InitNetworkClient();
    bool ConnectToServer(const std::string& host, enet_uint16 port);
    void DisconnectFromServer();
    bool TryConnect(const std::string& host, enet_uint16 port);
    void OnReceiveData() const;
    void ProcessEvent(const ENetEvent& event);
    static void InterpolateGameObjectStates(float delta_time);

};
} // namespace chroma::app::states